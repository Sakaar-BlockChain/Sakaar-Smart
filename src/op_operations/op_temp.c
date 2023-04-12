#include <stdio.h>
#include "sc_structs.h"
#include "sn_syntax.h"

#define Convert_Bool            0x01
#define Convert_Int             0x02
#define Convert_Float           0x03
#define Convert_Str             0x04

#define Delete_Scope_None       0x00
#define Delete_Scope_Func       0x01
#define Delete_Scope_Class      0x02

#define BlockType_Convert       0x01
#define BlockType_Arithmetic    0x02
#define BlockType_Set           0x03

#define BlockType_If_not        0x11
#define BlockType_If_not_del    0x12
#define BlockType_If            0x13
#define BlockType_If_del        0x14

#define BlockType_Put           0x21
#define BlockType_Delete_Temp   0x22
#define BlockType_Delete_Scope  0x23

#define BlockType_Continue      0x31
#define BlockType_Break         0x32
#define BlockType_Return        0x33

#define BlockType_List          0x41
#define BlockType_Attr          0x42
#define BlockType_Call          0x44
#define BlockType_Class         0x45


void run_an(struct op_state *state, struct object_st *object) {
    struct list_st *code_operations = state->code_operations;
    struct ast_node *node = object->data;
    struct op_attrib *attrib = NULL;
    struct op_block *new_block = NULL;

    if (node->main_type == MainType_Expr) {
        struct list_st *temp_list = node->next;
        switch (node->type) {
            case PrimType_List: {
                list_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = list_get_last(code_operations)->data;
                new_block->type = BlockType_List;
                new_block->count = temp_list->size;

                for (size_t i = 0; i < temp_list->size; i++) {
                    list_append(code_operations, temp_list->data[i]);
                }
                break;
            }
            case PrimType_Ident_new: {
                attrib = node->data->data;
                dlist_append(list_get_last(state->stack_memory)->data, node->data, attrib->data);
                op_attrib_new_data(attrib);
                list_append(state->temp_memory, attrib->data);
                break;
            }
            case PrimType_Ident_get: {
                attrib = node->data->data;
//                print_obj(attrib->data, 0);
                list_append(state->temp_memory, attrib->data);
                break;
            }
            case PrimType_Literal: {
                list_append(state->temp_memory, node->data);
                break;
            }
            case PrimType_Attrib:
//                printf("PrimType_Attrib ");
                break;
            case PrimType_Subscript:
//                printf("PrimType_Subscript ");
                break;
            case PrimType_Call: {
                list_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = list_get_last(code_operations)->data;
                new_block->type = BlockType_Call;
                new_block->count = (((struct ast_node *) temp_list->data[1]->data)->next)->size;

                list_append(code_operations, temp_list->data[0]);
                temp_list = ((struct ast_node *) temp_list->data[1]->data)->next;
                for (size_t i = 0; i < temp_list->size; i++) {
                    list_append(code_operations, temp_list->data[i]);
                }
                break;
            }
        }
    }
    if (node->main_type == MainType_Oper) {
        struct list_st *temp_list = node->tokens;
        size_t count = 2;
        if (node->type == ExprType_U || node->type == ExprType_NotTest) count = 1;
        int is_bool = 0;
        for (size_t i = 0; i < temp_list->size; i++) {
            list_add_new(code_operations, OP_BLOCK_TYPE);
            new_block = list_get_last(code_operations)->data;
            new_block->type = BlockType_Arithmetic;
            new_block->subtype = ((struct tk_token *) temp_list->data[i]->data)->subtype;
            new_block->count = count;

            if (((struct tk_token *) temp_list->data[i]->data)->subtype == Special_AND_AND ||
                ((struct tk_token *) temp_list->data[i]->data)->subtype == Special_OR_OR ||
                ((struct tk_token *) temp_list->data[i]->data)->subtype == Special_NOT)
                is_bool = 1;

        }
        temp_list = node->next;
        for (size_t i = temp_list->size; i > 0; i--) {
            if (is_bool) {
                list_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = list_get_last(code_operations)->data;
                new_block->type = BlockType_Convert;
                new_block->subtype = Convert_Bool;
            }
            list_append(code_operations, temp_list->data[i - 1]);
        }
    }
    if (node->main_type == MainType_Stmt) {
        struct list_st *temp_list = node->next;
        switch (node->type) {
            case StmtType_If: {
                // else
                if (temp_list->size % 2 == 1) {
                    list_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = list_get_last(code_operations)->data;
                    new_block->type = BlockType_If_not;
                    new_block->data1 = object_copy(temp_list->data[temp_list->size - 1]);
                }
                // else if
                for (size_t i = temp_list->size - (temp_list->size % 2) - 1; i >= 2; i -= 2) {
                    list_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = list_get_last(code_operations)->data;
                    new_block->type = BlockType_If_not_del;
                    {
                        struct object_st *obj = object_new();
                        object_set_type(obj, OP_BLOCK_TYPE);
                        ((struct op_block *) obj->data)->type = BlockType_If;
                        ((struct op_block *) obj->data)->data1 = object_copy(temp_list->data[i]);
                        new_block->data1 = obj;
                    }
                    new_block->data2 = object_copy(temp_list->data[i - 1]);
                }
                // if
                {
                    list_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = list_get_last(code_operations)->data;
                    new_block->type = BlockType_If;
                    new_block->data1 = object_copy(temp_list->data[1]);
                }
                list_append(code_operations, temp_list->data[0]);
                break;
            }
            case StmtType_While: {
                list_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = list_get_last(code_operations)->data;
                new_block->type = BlockType_If_del;
                new_block->data1 = object_copy(object);
                new_block->data2 = object_copy(temp_list->data[1]);

                list_append(code_operations, temp_list->data[0]);
                break;
            }
            case StmtType_DoWhile: {
                list_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = list_get_last(code_operations)->data;
                new_block->type = BlockType_If_del;
                new_block->data1 = object_copy(object);

                list_append(code_operations, temp_list->data[1]);
                list_append(code_operations, temp_list->data[0]);
                break;
            }
            case StmtType_Func_Body: {
                list_add_new(state->temp_memory, OP_OBJECT_TYPE);
                op_object_set_function(list_get_last(state->temp_memory)->data, node);
                break;
            }
            case StmtType_Assign: {
                temp_list = node->tokens;
                size_t count = 2;
                for (size_t i = 0; i < temp_list->size; i++) {
                    list_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = list_get_last(code_operations)->data;
                    new_block->type = BlockType_Arithmetic;
                    new_block->subtype = ((struct tk_token *) temp_list->data[i]->data)->subtype;
                    new_block->count = count;
                }
                temp_list = node->next;
                for (size_t i = temp_list->size; i > 0; i--) {
                    list_append(code_operations, temp_list->data[i - 1]);
                }
                break;
            }
            case StmtType_Func:
            case StmtType_Annot: {
                temp_list = node->next;
                if (temp_list->size == 2) {
                    list_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = list_get_last(code_operations)->data;
                    new_block->type = BlockType_Set;
                }
                for (size_t i = temp_list->size; i > 0; i--) {
                    list_append(code_operations, temp_list->data[i - 1]);
                }
                break;
            }
            case StmtType_Return: {
                list_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = list_get_last(code_operations)->data;
                new_block->type = BlockType_Return;
                new_block->count = temp_list->size;

                for (size_t i = 0; i < temp_list->size; i++) {
                    list_append(code_operations, temp_list->data[i]);
                }
                break;
            }
            case StmtType_Break:
//                printf("StmtType_Break ");
                break;
            case StmtType_Continue:
//                printf("StmtType_Continue ");
                break;
            case StmtType_List: {
                {
                    list_add_new(state->stack_memory, DLIST_TYPE);

                    list_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = list_get_last(code_operations)->data;
                    new_block->type = BlockType_Delete_Scope;
                    new_block->subtype = Delete_Scope_None;
                }

                for (size_t i = 0; i < temp_list->size; i++) {

                    list_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = list_get_last(code_operations)->data;
                    new_block->type = BlockType_Put;
                    {
                        struct object_st *obj = object_new();
                        object_set_type(obj, OP_BLOCK_TYPE);
                        ((struct op_block *) obj->data)->type = BlockType_Delete_Temp;
                        ((struct op_block *) obj->data)->count = state->temp_memory->size;
                        new_block->data1 = obj;
                    }
                    new_block->data2 = object_copy(
                            temp_list->data[temp_list->size - i - 1]);
                }
                break;
            }
            case StmtType_Extends:
//                printf("StmtType_Extends ");
                break;
        }
    }
//    printf("\n");
}

void run_op(struct op_state *state, struct object_st *object) {
    struct list_st *code_operations = state->code_operations;
    struct op_block *block = object->data;
    struct op_attrib *attrib = NULL;
    struct op_block *new_block = NULL;

    if ((block->type & 0xF0) == 0x10) {
        struct object_st *temp = object_new();
        struct object_st *obj = object_copy(list_get_last(state->temp_memory));

        if (obj->type != INTEGER_TYPE) {
            list_append(code_operations, object);

            list_add_new(code_operations, OP_BLOCK_TYPE);
            new_block = list_get_last(code_operations)->data;
            new_block->type = BlockType_Convert;
            new_block->subtype = Convert_Bool;
        } else {
            int res = 0;
            if ((block->type == BlockType_If_not || block->type == BlockType_If_not_del) &&
                integer_is_null(obj->data))
                res = 1;
            if ((block->type == BlockType_If || block->type == BlockType_If_del) && !integer_is_null(obj->data))
                res = 1;

            if (res) {
                if (block->type == BlockType_If_not_del || block->type == BlockType_If_del)
                    list_remove_last(state->temp_memory);
                list_append(code_operations, block->data1);
                list_append(code_operations, block->data2);
            }
        }
        object_free(obj);
        object_free(temp);
        return;
    }

    switch (block->type) {
        case BlockType_Convert: {
            struct object_st *temp = NULL;
            struct object_st *obj = object_copy(list_get_last(state->temp_memory));
            list_remove_last(state->temp_memory);

            if (obj->type == OP_OBJECT_TYPE) {
                struct string_st *ind_str = string_new();

                if (block->subtype == Convert_Bool) string_set_str(ind_str, "__bool__", 8);
                else if (block->subtype == Convert_Int) string_set_str(ind_str, "__int__", 7);
                else if (block->subtype == Convert_Float) string_set_str(ind_str, "__float__", 9);
                else if (block->subtype == Convert_Str) string_set_str(ind_str, "__str__", 7);

                temp = op_object_get_attrib(obj->data, ind_str);
                string_free(ind_str);
            }

            if (temp != NULL) {
                list_append(state->temp_memory, obj);
                list_append(state->temp_memory, temp);

                list_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = list_get_last(code_operations)->data;
                new_block->type = BlockType_Call;
                new_block->count = 1;
            } else {
                struct object_st *res = object_new();

                if (block->subtype == Convert_Bool) object__bool(res, obj);
                else if (block->subtype == Convert_Int) object__int(res, obj);
                else if (block->subtype == Convert_Float) object__float(res, obj);
                else if (block->subtype == Convert_Str) object__str(res, obj);

                list_append(state->temp_memory, res);
                object_free(res);
            }
            object_free(obj);
            object_free(temp);
            break;
        }
        case BlockType_Arithmetic: {
            if (block->count == 2) {
                if ((block->subtype & 0xF0) == 0x10) {
                    struct object_st *temp = NULL;
                    struct object_st *obj2 = object_copy(list_get_last(state->temp_memory));
                    list_remove_last(state->temp_memory);
                    struct object_st *obj1 = object_copy(list_get_last(state->temp_memory));
                    list_remove_last(state->temp_memory);

                    if (obj1->type == OP_OBJECT_TYPE) {
                        struct string_st *ind_str = string_new();

                        if (block->subtype == Special_MOD) string_set_str(ind_str, "__mod__", 7);
                        else if (block->subtype == Special_AND) string_set_str(ind_str, "__and__", 7);
                        else if (block->subtype == Special_MUL) string_set_str(ind_str, "__mul__", 7);
                        else if (block->subtype == Special_ADD) string_set_str(ind_str, "__add__", 7);
                        else if (block->subtype == Special_SUB) string_set_str(ind_str, "__sub__", 7);
                        else if (block->subtype == Special_DIV) string_set_str(ind_str, "__div__", 7);
                        else if (block->subtype == Special_XOR) string_set_str(ind_str, "__xor__", 7);
                        else if (block->subtype == Special_OR) string_set_str(ind_str, "__or__", 6);
                        else if (block->subtype == Special_LSHIFT) string_set_str(ind_str, "__ls__", 6);
                        else if (block->subtype == Special_RSHIFT) string_set_str(ind_str, "__rs__", 6);

                        temp = op_object_get_attrib(obj1->data, ind_str);
                        string_free(ind_str);
                    }

                    if (temp != NULL) {
                        list_append(state->temp_memory, obj2);
                        list_append(state->temp_memory, obj1);
                        list_append(state->temp_memory, temp);

                        list_add_new(code_operations, OP_BLOCK_TYPE);
                        new_block = list_get_last(code_operations)->data;
                        new_block->type = BlockType_Call;
                        new_block->count = 2;
                    } else {
                        struct object_st *res = object_new();

                        if (block->subtype == Special_MOD) object__mod(res, obj1, obj2);
                        else if (block->subtype == Special_AND) object__and(res, obj1, obj2);
                        else if (block->subtype == Special_MUL) object__mul(res, obj1, obj2);
                        else if (block->subtype == Special_ADD) object__add(res, obj1, obj2);
                        else if (block->subtype == Special_SUB) object__sub(res, obj1, obj2);
                        else if (block->subtype == Special_DIV) object__div(res, obj1, obj2);
                        else if (block->subtype == Special_XOR) object__xor(res, obj1, obj2);
                        else if (block->subtype == Special_OR) object__or(res, obj1, obj2);
                        else if (block->subtype == Special_LSHIFT) object__ls(res, obj1, obj2);
                        else if (block->subtype == Special_RSHIFT) object__rs(res, obj1, obj2);

                        list_append(state->temp_memory, res);
                        object_free(res);
                    }
                    object_free(temp);
                    object_free(obj2);
                    object_free(obj1);
                    break;
                } else if ((block->subtype & 0xF0) == 0x20) {
                    struct object_st *temp = NULL;
                    struct object_st *obj2 = object_copy(list_get_last(state->temp_memory));
                    list_remove_last(state->temp_memory);
                    struct object_st *obj1 = object_copy(list_get_last(state->temp_memory));
                    list_remove_last(state->temp_memory);

                    if (obj1->type == OP_OBJECT_TYPE) {
                        struct string_st *ind_str = string_new();

                        if (block->subtype == Special_EQ_MOD) string_set_str(ind_str, "__mod__", 7);
                        else if (block->subtype == Special_EQ_AND) string_set_str(ind_str, "__and__", 7);
                        else if (block->subtype == Special_EQ_MUL) string_set_str(ind_str, "__mul__", 7);
                        else if (block->subtype == Special_EQ_ADD) string_set_str(ind_str, "__add__", 7);
                        else if (block->subtype == Special_EQ_SUB) string_set_str(ind_str, "__sub__", 7);
                        else if (block->subtype == Special_EQ_DIV) string_set_str(ind_str, "__div__", 7);
                        else if (block->subtype == Special_EQ_XOR) string_set_str(ind_str, "__xor__", 7);
                        else if (block->subtype == Special_EQ_OR) string_set_str(ind_str, "__or__", 6);
                        else if (block->subtype == Special_EQ_LSHIFT) string_set_str(ind_str, "__ls__", 6);
                        else if (block->subtype == Special_EQ_RSHIFT) string_set_str(ind_str, "__rs__", 6);

                        temp = op_object_get_attrib(obj1->data, ind_str);
                        string_free(ind_str);
                    }

                    if (temp != NULL) {
                        list_append(state->temp_memory, obj1);
                        list_append(state->temp_memory, obj2);
                        list_append(state->temp_memory, obj1);
                        list_append(state->temp_memory, temp);

                        list_add_new(code_operations, OP_BLOCK_TYPE);
                        new_block = list_get_last(code_operations)->data;
                        new_block->type = BlockType_Set;

                        list_add_new(code_operations, OP_BLOCK_TYPE);
                        new_block = list_get_last(code_operations)->data;
                        new_block->type = BlockType_Call;
                        new_block->count = 2;
                    } else {
                        struct object_st *res = object_new();

                        if (block->subtype == Special_EQ_MOD) object__mod(res, obj1, obj2);
                        else if (block->subtype == Special_EQ_AND) object__and(res, obj1, obj2);
                        else if (block->subtype == Special_EQ_MUL) object__mul(res, obj1, obj2);
                        else if (block->subtype == Special_EQ_ADD) object__add(res, obj1, obj2);
                        else if (block->subtype == Special_EQ_SUB) object__sub(res, obj1, obj2);
                        else if (block->subtype == Special_EQ_DIV) object__div(res, obj1, obj2);
                        else if (block->subtype == Special_EQ_XOR) object__xor(res, obj1, obj2);
                        else if (block->subtype == Special_EQ_OR) object__or(res, obj1, obj2);
                        else if (block->subtype == Special_EQ_LSHIFT) object__ls(res, obj1, obj2);
                        else if (block->subtype == Special_EQ_RSHIFT) object__rs(res, obj1, obj2);

                        object_set(obj1, res);
                        list_append(state->temp_memory, obj1);
                        object_free(res);
                    }
                    object_free(temp);
                    object_free(obj2);
                    object_free(obj1);
                    break;
                } else if ((block->subtype & 0xF0) == 0x40) {
                    struct object_st *temp = NULL;
                    struct object_st *obj2 = object_copy(list_get_last(state->temp_memory));
                    list_remove_last(state->temp_memory);
                    struct object_st *obj1 = object_copy(list_get_last(state->temp_memory));
                    list_remove_last(state->temp_memory);

                    if (obj1->type == OP_OBJECT_TYPE) {
                        struct string_st *ind_str = string_new();


                        if (block->subtype == Special_LESS) string_set_str(ind_str, "__lt__", 6);
                        else if (block->subtype == Special_GREATER) string_set_str(ind_str, "__gt__", 6);
                        else if (block->subtype == Special_EQ_LESS) string_set_str(ind_str, "__le__", 6);
                        else if (block->subtype == Special_EQ_GREATER) string_set_str(ind_str, "__ge__", 6);
                        else if (block->subtype == Special_EQ_NOT || block->subtype == Special_EQ_EQ)
                            string_set_str(ind_str, "__eq__", 6);

                        temp = op_object_get_attrib(obj1->data, ind_str);
                        string_free(ind_str);
                    }

                    if (temp != NULL) {
                        list_append(state->temp_memory, obj2);
                        list_append(state->temp_memory, obj1);
                        list_append(state->temp_memory, temp);

                        if (block->subtype == Special_EQ_NOT) {
                            list_add_new(code_operations, OP_BLOCK_TYPE);
                            new_block = list_get_last(code_operations)->data;
                            new_block->type = BlockType_Arithmetic;
                            new_block->subtype = Special_NOT;
                            new_block->count = 1;
                        }

                        list_add_new(code_operations, OP_BLOCK_TYPE);
                        new_block = list_get_last(code_operations)->data;
                        new_block->type = BlockType_Call;
                        new_block->count = 2;
                    } else {
                        int cmp_res = object_cmp(obj1, obj2);

                        struct object_st *res = object_new();
                        object_set_type(res, INTEGER_TYPE);

                        if (
                                (block->subtype == Special_LESS && cmp_res < 0) ||
                                (block->subtype == Special_GREATER && cmp_res != 2 && cmp_res > 0) ||
                                (block->subtype == Special_EQ_LESS && cmp_res <= 0) ||
                                (block->subtype == Special_EQ_GREATER && cmp_res != 2 && cmp_res >= 0) ||
                                (block->subtype == Special_EQ_NOT && cmp_res != 0) ||
                                (block->subtype == Special_EQ_EQ && cmp_res == 0))
                            integer_set_ui(res->data, 1);
                        else integer_set_ui(res->data, 0);

                        list_append(state->temp_memory, res);
                        object_free(res);
                    }
                    object_free(temp);
                    object_free(obj2);
                    object_free(obj1);
                    break;
                } else if ((block->subtype & 0xF0) == 0x50) {
                    struct object_st *obj2 = object_copy(list_get_last(state->temp_memory));
                    list_remove_last(state->temp_memory);
                    struct object_st *obj1 = object_copy(list_get_last(state->temp_memory));
                    list_remove_last(state->temp_memory);

                    int bool1 = 1;
                    if (obj1->type == INTEGER_TYPE && integer_is_null(obj1->data)) bool1 = 0;
                    int bool2 = 1;
                    if (obj2->type == INTEGER_TYPE && integer_is_null(obj2->data)) bool2 = 0;
                    object_free(obj2);
                    object_free(obj1);

                    struct object_st *res = object_new();
                    object_set_type(res, INTEGER_TYPE);
                    if (block->subtype == Special_AND_AND) integer_set_ui(res->data, bool1 && bool2);
                    if (block->subtype == Special_OR_OR) integer_set_ui(res->data, bool1 || bool2);

                    list_append(state->temp_memory, res);
                    object_free(res);
                    break;
                }

            } else if (block->count == 1) {
                struct object_st *temp = NULL;
                struct object_st *obj = object_copy(list_get_last(state->temp_memory));
                list_remove_last(state->temp_memory);
                if (obj->type == OP_OBJECT_TYPE) {
                    struct string_st *ind_str = string_new();

                    string_set_str(ind_str, "__ne__", 6);

                    temp = op_object_get_attrib(obj->data, ind_str);
                    string_free(ind_str);
                }

                if (temp != NULL) {
                    list_append(state->temp_memory, obj);
                    list_append(state->temp_memory, temp);

                    list_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = list_get_last(code_operations)->data;
                    new_block->type = BlockType_Call;
                    new_block->count = 1;
                } else {
                    struct object_st *res = object_new();

                    object__neg(res, obj);

                    list_append(state->temp_memory, res);
                    object_free(res);
                }
                object_free(temp);
                object_free(obj);
                break;
            }
        }
        case BlockType_Set: {
            struct object_st *temp = NULL;
            struct object_st *obj2 = object_copy(list_get_last(state->temp_memory));
            list_remove_last(state->temp_memory);
            struct object_st *obj1 = object_copy(list_get_last(state->temp_memory));
            list_remove_last(state->temp_memory);

            if (obj1->type == OP_OBJECT_TYPE) {
                struct string_st *ind_str = string_new();
                string_set_str(ind_str, "__set__", 7);
                temp = op_object_get_attrib(obj1->data, ind_str);
                string_free(ind_str);
            }
            if (temp != NULL) {
                list_append(state->temp_memory, obj2);
                list_append(state->temp_memory, obj1);
                list_append(state->temp_memory, temp);

                list_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = list_get_last(code_operations)->data;
                new_block->type = BlockType_Call;
                new_block->count = 2;
            } else {
                object_set(obj1, obj2);
                list_append(state->temp_memory, obj2);
            }

            object_free(temp);
            object_free(obj2);
            object_free(obj1);
            break;
        }

        case BlockType_Put: {
            list_append(code_operations, block->data1);
            list_append(code_operations, block->data2);
            break;
        }
        case BlockType_Delete_Temp: {
            if (state->temp_memory->size > block->count) list_resize(state->temp_memory, block->count);
            break;
        }
        case BlockType_Delete_Scope: {
            if (block->subtype == Delete_Scope_Func) {
                if (state->return_obj == NULL) list_add_new(state->temp_memory, NONE_TYPE);
                else {
                    list_append(state->temp_memory, state->return_obj);
                    object_free(state->return_obj);
                    state->return_obj = NULL;
                }
            }
//            else if (block->subtype == Delete_Scope_Class) {
//                object_set(list_get_last(state->temp_memory), state->memory->top->data);
//            }
            struct dlist_st *closure = list_get_last(state->stack_memory)->data;
            for (size_t i = 0; i < closure->size; i++) {
                attrib = closure->data[0][i]->data;
                object_free(attrib->data);
                attrib->data = object_copy(closure->data[1][i]);
            }
            list_remove_last(state->stack_memory);
            break;
        }

        case BlockType_Continue: {
//            struct object_st *current_object = NULL;
//            while (code_operations->size) {
//                current_object = code_operations->top->data;
//
//
//                if (current_object->type == OP_BLOCK_TYPE) {
//                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Temp &&
//                        state->temp_memory->size > ((struct op_block *) current_object->data)->count)
//                        list_resize(state->temp_memory, block->count);
//                } else if (current_object->type == AST_NODE_TYPE) {
//                    if (((struct ast_node *) current_object->data)->type == StmtType_While) {
//                        break;
//                    }// TODO for and do while loop continue
//                }
//                stack_pop(code_operations);
//            }
            break;
        }
        case BlockType_Break: {
//            struct object_st *current_object = NULL;
//            while (code_operations->size) {
//                current_object = code_operations->top->data;
//                if (current_object->type == OP_BLOCK_TYPE) {
//                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Temp &&
//                        state->temp_memory->size > ((struct op_block *) current_object->data)->count)
//                        list_resize(state->temp_memory, block->count);
//                } else if (current_object->type == AST_NODE_TYPE) {
//                    if (((struct ast_node *) current_object->data)->type == StmtType_While) {
//                        stack_pop(code_operations);
//                        break;
//                    }// TODO for and do while loop break
//                }
//                stack_pop(code_operations);
//            }
            break;
        }
        case BlockType_Return: {
            if (block->count != 0 && state->return_obj == NULL) {
                if (block->count == 1) {
                    state->return_obj = object_copy(list_get_last(state->temp_memory));
                    list_remove_last(state->temp_memory);
                } else {
                    state->return_obj = object_new();
                    object_set_type(state->return_obj, LIST_TYPE);
                    for (size_t i = 0; i < block->count; i++) {
                        list_append(state->return_obj->data, list_get_last(state->temp_memory));
                        list_remove_last(state->temp_memory);
                    }
                }
            }
            struct object_st *current_object = NULL;
            while (code_operations->size) {
                current_object = list_get_last(code_operations);
                if (current_object->type == OP_BLOCK_TYPE) {
                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Temp &&
                        state->temp_memory->size > ((struct op_block *) current_object->data)->count)
                        list_remove_last(state->temp_memory);
                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Scope) break;
                }
                list_remove_last(code_operations);
            }
            break;
        }

        case BlockType_List: {
            struct object_st *obj = object_new();
            object_set_type(obj, LIST_TYPE);
            for (size_t i = 0; i < block->count; i++) {
                list_append(obj->data, list_get_last(state->temp_memory));
                list_remove_last(state->temp_memory);
            }
            list_append(state->temp_memory, obj);
            object_free(obj);
            break;
        }
        case BlockType_Attr: {
//            struct object_st *res = NULL;
//            struct string_st *ind_str = ((struct ast_node *) block->data1->data)->data->data;
//            struct object_st *obj = object_copy(list_get_last(state->temp_memory));
//            if (code_operations->top->data->type != OP_BLOCK_TYPE ||
//                ((struct op_block *) code_operations->top->data->data)->type != BlockType_Call) {
//                list_remove_last(state->temp_memory);
//            } else {
//                ((struct op_block *) code_operations->top->data->data)->count++;
//            }
//
//            if (obj->type == MAP_TYPE) {
//                res = map_set_elm(obj->data, ind_str->data, ind_str->size);
//                list_append(state->temp_memory, res);
//            } else {
//                list_add_new(state->temp_memory, NONE_TYPE);
//            }
//
//            object_free(res);
//            object_free(obj);
            break;
        }
        case BlockType_Call: {
            struct object_st *res = NULL;
            struct string_st *ind_str = string_new();
            struct object_st *func = object_copy(list_get_last(state->temp_memory));
            list_remove_last(state->temp_memory);


            {
                list_add_new(state->stack_memory, DLIST_TYPE);

                list_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = list_get_last(code_operations)->data;
                new_block->type = BlockType_Delete_Scope;
                new_block->subtype = Delete_Scope_Func;
            }

            if (func->type == OP_OBJECT_TYPE) {
                string_set_str(ind_str, "__init__", 8);
                res = op_object_get_attrib(func->data, ind_str);
            }
            int ok = 0;
            if (res == NULL) {
                if (func->type == OP_OBJECT_TYPE) {
                    string_set_str(ind_str, "__params__", 10);
                    res = op_object_get_attrib(func->data, ind_str);
                }
                if (res != NULL) {
                    struct list_st *temp_list = ((struct ast_node *) res->data)->next;
                    struct ast_node *node = NULL;
                    if (temp_list->size == block->count) {
                        ok = 1;
                        for (int i = 0; i < temp_list->size; i++) {
                            node = temp_list->data[i]->data;

                            attrib = node->data->data;
                            dlist_append(list_get_last(state->stack_memory)->data, node->data, attrib->data);
                            op_attrib_new_data(attrib);
                            object_set(attrib->data, list_get_last(state->temp_memory));
                            list_remove_last(state->temp_memory);
                        }
                    }
                }
                object_free(res);
                res = NULL;

                if (func->type == OP_OBJECT_TYPE && ok) {
                    string_set_str(ind_str, "__closure__", 11);
                    res = op_object_get_attrib(func->data, ind_str);
                }
                if (res != NULL && ok) {
                    struct dlist_st *temp_list = res->data;

                    for (int i = 0; i < temp_list->size; i++) {
                        attrib = temp_list->data[0][i]->data;
                        dlist_append(list_get_last(state->stack_memory)->data, temp_list->data[0][i], attrib->data);
                        op_attrib_set_data(attrib, temp_list->data[1][i]);
                    }
                }
                object_free(res);
                res = NULL;

                if (func->type == OP_OBJECT_TYPE && ok) {
                    string_set_str(ind_str, "__call__", 7);
                    res = op_object_get_attrib(func->data, ind_str);
                }

                if (res != NULL && ok) {
                    list_append(code_operations, res);
                }
                object_free(res);
            } else {
//                struct object_st *_res = NULL;
//                struct object_st *elm = NULL;
//                if (res->type == MAP_TYPE) {
//                    string_set_str(ind_str, "__params__", 10);
//                    _res = op_object_get_attrib(res->data, ind_str);
//                }
//                if (res != NULL) {
//                    struct string_st *ind_temp = NULL;
//                    struct list_st *temp_list = ((struct ast_node *) _res->data)->next;
//                    if (temp_list->size == block->count + 1) {
//                        ind_temp = ((struct ast_node *) temp_list->data[0]->data)->data->data;
//                        elm = map_set_elm(state->memory->top->data->data, ind_temp->data, ind_temp->size);
//                        state->return_obj = object_copy(elm);
//                        object_set(elm, func);
//                        object_free(elm);
//                    }
//
//                    if (temp_list->size == block->count + 1) {
//                        ok = 1;
//                        for (int i = 1; i < temp_list->size; i++) {
//                            ind_temp = ((struct ast_node *) temp_list->data[i]->data)->data->data;
//                            elm = map_set_elm(state->memory->top->data->data, ind_temp->data, ind_temp->size);
//                            object_set(elm, list_get_last(state->temp_memory));
//                            list_remove_last(state->temp_memory);
//                            object_free(elm);
//                        }
//                    }
//                }
//                object_free(_res);
//                _res = NULL;
//                if (res->type == MAP_TYPE && ok) {
//                    string_set_str(ind_str, "__call__", 7);
//                    sha256_code._code(ind_str, ind_str);
//                    _res = op_object_get_attrib(res->data, ind_str->data, ind_str->size);
//                    list_append(code_operations, _res);
//                }
//                object_free(_res);
//                object_free(res);
            }

            string_free(ind_str);
            object_free(func);
            break;
        }
        case BlockType_Class: {
//            list_add_new(state->memory, MAP_TYPE);
            break;
        }

    }
}

void run_smart_contract(struct object_st *expr_obj) {
    struct op_state *state = op_state_new();
    list_append(state->code_operations, expr_obj);
    {
        struct list_st *code_operations = state->code_operations;
        struct object_st *current_object = NULL;
        while (code_operations->size) {
            current_object = object_copy(list_get_last(code_operations));
            list_remove_last(code_operations);

            if (current_object->type == AST_NODE_TYPE) {
                run_an(state, current_object);
            }
            if (current_object->type == OP_BLOCK_TYPE) {
                run_op(state, current_object);
            }
            object_free(current_object);
        }
    }
    op_state_free(state);
}
