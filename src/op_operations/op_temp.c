#include "sc_structs.h"
#include "sn_syntax.h"

#define Convert_Bool            0x01
#define Convert_Int             0x02
#define Convert_Float           0x03
#define Convert_Str             0x04

#define Delete_Scope_Func       0x01
#define Delete_Scope_Class      0x02

#define BlockType_Convert       0x01
#define BlockType_Arithmetic    0x02

#define BlockType_If_not        0x11
#define BlockType_If_not_del    0x12
#define BlockType_If            0x13
#define BlockType_If_del        0x14

#define BlockType_Put           0x03
#define BlockType_Delete_Temp   0x04
#define BlockType_Delete_Scope  0x05

#define BlockType_Continue      0x06
#define BlockType_Break         0x07
#define BlockType_Return        0x08

#define BlockType_List          0x09
#define BlockType_Attr          0x0a
#define BlockType_Func          0x0b
#define BlockType_Call          0x0c
#define BlockType_Class         0x0e


//void run_an(struct op_state *state, struct object_st *object) {
//    struct stack_st *code_operations = state->code_operations;
//    struct ast_node *node = object->data;
//
//    if (node->main_type == MainType_Expr) {
//        struct list_st *temp_list = node->next;
//        switch (node->type) {
//            case PrimType_List: {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_List;
//                ((struct op_block *) code_operations->top->data->data)->count = temp_list->size;
//
//                for (size_t i = 0; i < temp_list->size; i++) {
//                    stack_add(code_operations, temp_list->data[i]);
//                }
//                break;
//            }
//            case PrimType_Identifier: {
//                struct object_st *res = NULL;
//                struct string_st *ind_str = node->data->data;
//                res = map_set_elm(state->memory->top->data->data, ind_str->data, ind_str->size);
//                stack_add(state->temp_memory, res);
//                print_obj(res, 0);
//                object_free(res);
//                break;
//            }
//            case PrimType_Literal: {
//                stack_add(state->temp_memory, node->data);
//                break;
//            }
//            case PrimType_Attrib: {
//                struct object_st *res = NULL;
//                struct string_st *ind_str = node->data->data;
//                res = map_set_elm(state->memory->top->data->data, ind_str->data, ind_str->size);
//                stack_add(state->temp_memory, res);
//                object_free(res);
//                for (size_t i = temp_list->size; i > 0; i--) {
//                    stack_add_new(code_operations, OP_BLOCK_TYPE);
//                    ((struct op_block *) code_operations->top->data->data)->type = BlockType_Attr;
//                    ((struct op_block *) code_operations->top->data->data)->data1 = object_copy(
//                            temp_list->data[i - 1]);
//                }
//                break;
//            }
//            case PrimType_Subscript:
//                printf("PrimType_Subscript ");
//                break;
//            case PrimType_Call: {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_Call;
//                ((struct op_block *) code_operations->top->data->data)->count = (((struct ast_node *) temp_list->data[1]->data)->next)->size;
//                stack_add(code_operations, temp_list->data[0]);
//                temp_list = ((struct ast_node *) temp_list->data[1]->data)->next;
//                for (size_t i = 0; i < temp_list->size; i++) {
//                    stack_add(code_operations, temp_list->data[i]);
//                }
//                break;
//            }
//        }
//    }
//    if (node->main_type == MainType_Oper) {
//        struct list_st *temp_list = node->tokens;
//        size_t count = 2;
//        if (node->type == ExprType_U || node->type == ExprType_NotTest) count = 1;
//        int is_bool = 0;
//        for (size_t i = 0; i < temp_list->size; i++) {
//            stack_add_new(code_operations, OP_BLOCK_TYPE);
//            ((struct op_block *) code_operations->top->data->data)->type = BlockType_Arithmetic;
//            ((struct op_block *) code_operations->top->data->data)->subtype = ((struct tk_token *) temp_list->data[i]->data)->subtype;
//            ((struct op_block *) code_operations->top->data->data)->count = count;
//
//            if (((struct tk_token *) temp_list->data[i]->data)->subtype == Special_AND_AND ||
//                ((struct tk_token *) temp_list->data[i]->data)->subtype == Special_OR_OR ||
//                ((struct tk_token *) temp_list->data[i]->data)->subtype == Special_NOT)
//                is_bool = 1;
//
//        }
//        temp_list = node->next;
//        for (size_t i = temp_list->size; i > 0; i--) {
//            if (is_bool) {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_Convert;
//                ((struct op_block *) code_operations->top->data->data)->subtype = Convert_Bool;
//            }
//            stack_add(code_operations, temp_list->data[i - 1]);
//        }
//    }
//    if (node->main_type == MainType_Stmt) {
//        struct list_st *temp_list = node->next;
//        switch (node->type) {
//            case StmtType_If: {
//                // else
//                if (temp_list->size % 2 == 1) {
//                    stack_add_new(code_operations, OP_BLOCK_TYPE);
//                    ((struct op_block *) code_operations->top->data->data)->type = BlockType_If_not;
//                    ((struct op_block *) code_operations->top->data->data)->data1 = object_copy(
//                            temp_list->data[temp_list->size - 1]);
//                }
//                // else if
//                for (size_t i = temp_list->size - (temp_list->size % 2) - 1; i >= 2; i -= 2) {
//                    stack_add_new(code_operations, OP_BLOCK_TYPE);
//                    ((struct op_block *) code_operations->top->data->data)->type = BlockType_If_not_del;
//                    {
//                        struct object_st *obj = object_new();
//                        object_set_type(obj, OP_BLOCK_TYPE);
//                        ((struct op_block *) obj->data)->type = BlockType_If;
//                        ((struct op_block *) obj->data)->data1 = object_copy(temp_list->data[i]);
//                        ((struct op_block *) code_operations->top->data->data)->data1 = obj;
//                    }
//                    ((struct op_block *) code_operations->top->data->data)->data2 = object_copy(temp_list->data[i - 1]);
//                }
//                // if
//                {
//                    stack_add_new(code_operations, OP_BLOCK_TYPE);
//                    ((struct op_block *) code_operations->top->data->data)->type = BlockType_If;
//                    ((struct op_block *) code_operations->top->data->data)->data1 = object_copy(temp_list->data[1]);
//                }
//                stack_add(code_operations, temp_list->data[0]);
//                break;
//            }
//            case StmtType_While: {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_If_del;
//                ((struct op_block *) code_operations->top->data->data)->data1 = object_copy(object);
//                ((struct op_block *) code_operations->top->data->data)->data2 = object_copy(temp_list->data[1]);
//
//                stack_add(code_operations, temp_list->data[0]);
//                break;
//            }
//            case StmtType_DoWhile: {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_If_del;
//                ((struct op_block *) code_operations->top->data->data)->data1 = object_copy(object);
//
//                stack_add(code_operations, temp_list->data[1]);
//                stack_add(code_operations, temp_list->data[0]);
//                break;
//            }
//            case StmtType_Func: {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_Func;
//                ((struct op_block *) code_operations->top->data->data)->data1 = object_copy(
//                        temp_list->data[1]);
//                ((struct op_block *) code_operations->top->data->data)->data2 = object_copy(
//                        temp_list->data[2]);
//                stack_add(code_operations, temp_list->data[0]);
//                break;
//            }
//            case StmtType_Class: {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_Delete_Scope;
//                ((struct op_block *) code_operations->top->data->data)->subtype = Delete_Scope_Class;
//                ((struct op_block *) code_operations->top->data->data)->count = state->memory->size;
//
//                stack_add(code_operations, temp_list->data[1]);
//
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_Class;
//
//                stack_add(code_operations, temp_list->data[0]);
//                break;
//            }
//            case StmtType_Assign: {
//                temp_list = node->tokens;
//                size_t count = 2;
//                for (size_t i = 0; i < temp_list->size; i++) {
//                    stack_add_new(code_operations, OP_BLOCK_TYPE);
//                    ((struct op_block *) code_operations->top->data->data)->type = BlockType_Arithmetic;
//                    ((struct op_block *) code_operations->top->data->data)->subtype = ((struct tk_token *) temp_list->data[i]->data)->subtype;
//                    ((struct op_block *) code_operations->top->data->data)->count = count;
//                }
//                temp_list = node->next;
//                for (size_t i = temp_list->size; i > 0; i--) {
//                    stack_add(code_operations, temp_list->data[i - 1]);
//                }
//                //                            print_stack(code_operations, 0);
//                break;
//            }
//            case StmtType_Return: {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_Return;
//                ((struct op_block *) code_operations->top->data->data)->count = temp_list->size;
//
//                for (size_t i = 0; i < temp_list->size; i++) {
//                    stack_add(code_operations, temp_list->data[i]);
//                }
//                break;
//            }
//            case StmtType_Break: {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_Break;
//                break;
//            }
//            case StmtType_Continue: {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_Continue;
//                break;
//            }
//            case StmtType_List: {
//                for (size_t i = 0; i < temp_list->size; i++) {
//
//                    stack_add_new(code_operations, OP_BLOCK_TYPE);
//
//                    ((struct op_block *) code_operations->top->data->data)->type = BlockType_Put;
//                    {
//                        struct object_st *obj = object_new();
//                        object_set_type(obj, OP_BLOCK_TYPE);
//                        ((struct op_block *) obj->data)->type = BlockType_Delete_Temp;
//                        ((struct op_block *) obj->data)->count = state->temp_memory->size;
//                        ((struct op_block *) code_operations->top->data->data)->data1 = obj;
//                    }
//                    ((struct op_block *) code_operations->top->data->data)->data2 = object_copy(
//                            temp_list->data[temp_list->size - i - 1]);
//                }
//                break;
//            }
//            case StmtType_Extends:
//                printf("StmtType_Extends ");
//                break;
//        }
//    }
//}
//
//void run_op(struct op_state *state, struct object_st *object) {
//    struct stack_st *code_operations = state->code_operations;
//    struct op_block *block = object->data;
//    if ((block->type & 0xF0) == 0x10) {
//
//        struct object_st *temp = object_new();
//        struct object_st *obj = object_copy(state->temp_memory->top->data);
//
//        if (obj->type != INTEGER_TYPE) {
//            stack_add(code_operations, object);
//
//            stack_add_new(code_operations, OP_BLOCK_TYPE);
//            ((struct op_block *) code_operations->top->data->data)->type = BlockType_Convert;
//            ((struct op_block *) code_operations->top->data->data)->subtype = Convert_Bool;
//        } else {
//            int res = 0;
//            if ((block->type == BlockType_If_not || block->type == BlockType_If_not_del) &&
//                integer_is_null(obj->data))
//                res = 1;
//            if ((block->type == BlockType_If || block->type == BlockType_If_del) && !integer_is_null(obj->data))
//                res = 1;
//
//            if (res) {
//                if (block->type == BlockType_If_not_del || block->type == BlockType_If_del)
//                    stack_pop(state->temp_memory);
//                stack_add(code_operations, block->data1);
//                stack_add(code_operations, block->data2);
//            }
//        }
//        object_free(obj);
//        object_free(temp);
//        return;
//    }
//
//    switch (block->type) {
//        case BlockType_Convert: {
//            struct object_st *temp = NULL;
//            struct object_st *obj = object_copy(state->temp_memory->top->data);
//            if (obj->type == MAP_TYPE) {
//                struct string_st *ind_str = string_new();
//
//                if (block->subtype == Convert_Bool) string_set_str(ind_str, "__bool__", 8);
//                else if (block->subtype == Convert_Int) string_set_str(ind_str, "__int__", 7);
//                else if (block->subtype == Convert_Float) string_set_str(ind_str, "__float__", 9);
//                else if (block->subtype == Convert_Str) string_set_str(ind_str, "__str__", 7);
//
//                sha256_code._code(ind_str, ind_str);
//                temp = map_get_elm(obj->data, ind_str->data, ind_str->size);
//                string_free(ind_str);
//            }
//
//            if (temp == NULL) {
//                struct object_st *res = object_new();
//                stack_pop(state->temp_memory);
//
//                if (block->subtype == Convert_Bool) object__bool(res, obj);
//                else if (block->subtype == Convert_Int) object__int(res, obj);
//                else if (block->subtype == Convert_Float) object__float(res, obj);
//                else if (block->subtype == Convert_Str) object__str(res, obj);
//
//                stack_add(state->temp_memory, res);
//                object_free(res);
//            } else {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_Call;
//                ((struct op_block *) code_operations->top->data->data)->count = 1;
//
//                stack_add(state->temp_memory, temp);
//            }
//            object_free(obj);
//            object_free(temp);
//            break;
//        }
//        case BlockType_Arithmetic: {
//            if (block->count == 2) {
//                if ((block->subtype & 0xF0) == 0x10) {
//                    struct object_st *temp = NULL;
//                    struct object_st *obj2 = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//                    struct object_st *obj1 = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//
//                    if (obj1->type == MAP_TYPE) {
//                        struct string_st *ind_str = string_new();
//
//                        if (block->subtype == Special_MOD) string_set_str(ind_str, "__mod__", 7);
//                        else if (block->subtype == Special_AND) string_set_str(ind_str, "__and__", 7);
//                        else if (block->subtype == Special_MUL) string_set_str(ind_str, "__mul__", 7);
//                        else if (block->subtype == Special_ADD) string_set_str(ind_str, "__add__", 7);
//                        else if (block->subtype == Special_SUB) string_set_str(ind_str, "__sub__", 7);
//                        else if (block->subtype == Special_DIV) string_set_str(ind_str, "__div__", 7);
//                        else if (block->subtype == Special_XOR) string_set_str(ind_str, "__xor__", 7);
//                        else if (block->subtype == Special_OR) string_set_str(ind_str, "__or__", 6);
//                        else if (block->subtype == Special_LSHIFT) string_set_str(ind_str, "__ls__", 6);
//                        else if (block->subtype == Special_RSHIFT) string_set_str(ind_str, "__rs__", 6);
//
//                        sha256_code._code(ind_str, ind_str);
//                        temp = map_get_elm(obj1->data, ind_str->data, ind_str->size);
//                        string_free(ind_str);
//                    }
//
//                    if (temp != NULL) {
//                        stack_add(state->temp_memory, obj2);
//                        stack_add(state->temp_memory, obj1);
//                        stack_add(state->temp_memory, temp);
//
//                        stack_add_new(code_operations, OP_BLOCK_TYPE);
//                        ((struct op_block *) code_operations->top->data->data)->type = BlockType_Call;
//                        ((struct op_block *) code_operations->top->data->data)->count = 2;
//                    } else {
//                        struct object_st *res = object_new();
//
//                        if (block->subtype == Special_MOD) object__mod(res, obj1, obj2);
//                        else if (block->subtype == Special_AND) object__and(res, obj1, obj2);
//                        else if (block->subtype == Special_MUL) object__mul(res, obj1, obj2);
//                        else if (block->subtype == Special_ADD) object__add(res, obj1, obj2);
//                        else if (block->subtype == Special_SUB) object__sub(res, obj1, obj2);
//                        else if (block->subtype == Special_DIV) object__div(res, obj1, obj2);
//                        else if (block->subtype == Special_XOR) object__xor(res, obj1, obj2);
//                        else if (block->subtype == Special_OR) object__or(res, obj1, obj2);
//                        else if (block->subtype == Special_LSHIFT) object__ls(res, obj1, obj2);
//                        else if (block->subtype == Special_RSHIFT) object__rs(res, obj1, obj2);
//
//                        stack_add(state->temp_memory, res);
//                        object_free(res);
//                    }
//                    object_free(temp);
//                    object_free(obj2);
//                    object_free(obj1);
//                    break;
//                } else if ((block->subtype & 0xF0) == 0x20) {
//                    struct object_st *temp = NULL;
//                    struct object_st *obj2 = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//                    struct object_st *obj1 = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//                    if (obj1->type == MAP_TYPE) {
//                        struct string_st *ind_str = string_new();
//
//                        if (block->subtype == Special_EQ_MOD) string_set_str(ind_str, "__mod__", 7);
//                        else if (block->subtype == Special_EQ_AND) string_set_str(ind_str, "__and__", 7);
//                        else if (block->subtype == Special_EQ_MUL) string_set_str(ind_str, "__mul__", 7);
//                        else if (block->subtype == Special_EQ_ADD) string_set_str(ind_str, "__add__", 7);
//                        else if (block->subtype == Special_EQ_SUB) string_set_str(ind_str, "__sub__", 7);
//                        else if (block->subtype == Special_EQ_DIV) string_set_str(ind_str, "__div__", 7);
//                        else if (block->subtype == Special_EQ_XOR) string_set_str(ind_str, "__xor__", 7);
//                        else if (block->subtype == Special_EQ_OR) string_set_str(ind_str, "__or__", 6);
//                        else if (block->subtype == Special_EQ_LSHIFT) string_set_str(ind_str, "__ls__", 6);
//                        else if (block->subtype == Special_EQ_RSHIFT) string_set_str(ind_str, "__rs__", 6);
//
//                        sha256_code._code(ind_str, ind_str);
//                        temp = map_get_elm(obj1->data, ind_str->data, ind_str->size);
//                        string_free(ind_str);
//                    }
//
//                    if (temp != NULL) {
//                        stack_add(state->temp_memory, obj1);
//                        stack_add(state->temp_memory, obj2);
//                        stack_add(state->temp_memory, obj1);
//                        stack_add(state->temp_memory, temp);
//
//                        stack_add_new(code_operations, OP_BLOCK_TYPE);
//                        ((struct op_block *) code_operations->top->data->data)->type = BlockType_Arithmetic;
//                        ((struct op_block *) code_operations->top->data->data)->subtype = Special_EQ;
//                        ((struct op_block *) code_operations->top->data->data)->count = 2;
//
//                        stack_add_new(code_operations, OP_BLOCK_TYPE);
//                        ((struct op_block *) code_operations->top->data->data)->type = BlockType_Call;
//                        ((struct op_block *) code_operations->top->data->data)->count = 2;
//                    } else {
//                        struct object_st *res = object_new();
//
//                        if (block->subtype == Special_EQ_MOD) object__mod(res, obj1, obj2);
//                        else if (block->subtype == Special_EQ_AND) object__and(res, obj1, obj2);
//                        else if (block->subtype == Special_EQ_MUL) object__mul(res, obj1, obj2);
//                        else if (block->subtype == Special_EQ_ADD) object__add(res, obj1, obj2);
//                        else if (block->subtype == Special_EQ_SUB) object__sub(res, obj1, obj2);
//                        else if (block->subtype == Special_EQ_DIV) object__div(res, obj1, obj2);
//                        else if (block->subtype == Special_EQ_XOR) object__xor(res, obj1, obj2);
//                        else if (block->subtype == Special_EQ_OR) object__or(res, obj1, obj2);
//                        else if (block->subtype == Special_EQ_LSHIFT) object__ls(res, obj1, obj2);
//                        else if (block->subtype == Special_EQ_RSHIFT) object__rs(res, obj1, obj2);
//
//                        object_set(obj1, res);
//                        stack_add(state->temp_memory, obj1);
//                        object_free(res);
//                    }
//                    object_free(temp);
//                    object_free(obj2);
//                    object_free(obj1);
//                    break;
//                } else if ((block->subtype & 0xF0) == 0x40) {
//                    struct object_st *temp = NULL;
//                    struct object_st *obj2 = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//                    struct object_st *obj1 = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//
//                    if (obj1->type == MAP_TYPE) {
//                        struct string_st *ind_str = string_new();
//
//
//                        if (block->subtype == Special_LESS) string_set_str(ind_str, "__lt__", 6);
//                        else if (block->subtype == Special_GREATER) string_set_str(ind_str, "__gt__", 6);
//                        else if (block->subtype == Special_EQ_LESS) string_set_str(ind_str, "__le__", 6);
//                        else if (block->subtype == Special_EQ_GREATER) string_set_str(ind_str, "__ge__", 6);
//                        else if (block->subtype == Special_EQ_NOT || block->subtype == Special_EQ_EQ)
//                            string_set_str(ind_str, "__eq__", 6);
//
//
//                        sha256_code._code(ind_str, ind_str);
//                        temp = map_get_elm(obj1->data, ind_str->data, ind_str->size);
//                        string_free(ind_str);
//                    }
//
//                    if (temp != NULL) {
//                        stack_add(state->temp_memory, obj2);
//                        stack_add(state->temp_memory, obj1);
//                        stack_add(state->temp_memory, temp);
//
//                        if (block->subtype == Special_EQ_NOT) {
//                            stack_add_new(code_operations, OP_BLOCK_TYPE);
//                            ((struct op_block *) code_operations->top->data->data)->type = BlockType_Arithmetic;
//                            ((struct op_block *) code_operations->top->data->data)->subtype = Special_NOT;
//                            ((struct op_block *) code_operations->top->data->data)->count = 1;
//                        }
//
//                        stack_add_new(code_operations, OP_BLOCK_TYPE);
//                        ((struct op_block *) code_operations->top->data->data)->type = BlockType_Call;
//                        ((struct op_block *) code_operations->top->data->data)->count = 2;
//                    } else {
//                        int cmp_res = object_cmp(obj1, obj2);
//
//                        struct object_st *res = object_new();
//                        object_set_type(res, INTEGER_TYPE);
//
//                        if (
//                                (block->subtype == Special_LESS && cmp_res < 0) ||
//                                (block->subtype == Special_GREATER && cmp_res != 2 && cmp_res > 0) ||
//                                (block->subtype == Special_EQ_LESS && cmp_res <= 0) ||
//                                (block->subtype == Special_EQ_GREATER && cmp_res != 2 && cmp_res >= 0) ||
//                                (block->subtype == Special_EQ_NOT && cmp_res != 0) ||
//                                (block->subtype == Special_EQ_EQ && cmp_res == 0))
//                            integer_set_ui(res->data, 1);
//                        else integer_set_ui(res->data, 0);
//
//                        stack_add(state->temp_memory, res);
//                        object_free(res);
//                    }
//                    object_free(temp);
//                    object_free(obj2);
//                    object_free(obj1);
//                    break;
//                } else if ((block->subtype & 0xF0) == 0x50) {
//                    struct object_st *obj2 = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//                    struct object_st *obj1 = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//
//                    int bool1 = 1;
//                    if (obj1->type == INTEGER_TYPE && integer_is_null(obj1->data)) bool1 = 0;
//                    int bool2 = 1;
//                    if (obj2->type == INTEGER_TYPE && integer_is_null(obj2->data)) bool2 = 0;
//                    object_free(obj2);
//                    object_free(obj1);
//
//                    struct object_st *res = object_new();
//                    object_set_type(res, INTEGER_TYPE);
//                    if (block->subtype == Special_AND_AND) integer_set_ui(res->data, bool1 && bool2);
//                    if (block->subtype == Special_OR_OR) integer_set_ui(res->data, bool1 || bool2);
//
//                    stack_add(state->temp_memory, res);
//                    object_free(res);
//                    break;
//                } else {
//                    struct object_st *temp = NULL;
//                    struct object_st *obj2 = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//                    struct object_st *obj1 = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//
//                    if (obj1->type == MAP_TYPE) {
//                        struct string_st *ind_str = string_new();
//
//                        string_set_str(ind_str, "__set__", 7);
//
//                        sha256_code._code(ind_str, ind_str);
//                        temp = map_get_elm(obj1->data, ind_str->data, ind_str->size);
//                        string_free(ind_str);
//                    }
//                    if (temp != NULL) {
//                        stack_add(state->temp_memory, obj2);
//                        stack_add(state->temp_memory, obj1);
//                        stack_add(state->temp_memory, temp);
//
//                        stack_add_new(code_operations, OP_BLOCK_TYPE);
//                        ((struct op_block *) code_operations->top->data->data)->type = BlockType_Call;
//                        ((struct op_block *) code_operations->top->data->data)->count = 2;
//                    } else {
//                        object_set(obj1, obj2);
//                        stack_add(state->temp_memory, obj2);
//                    }
//
//                    object_free(temp);
//                    object_free(obj2);
//                    object_free(obj1);
//                    break;
//                }
//
//
//            } else if (block->count == 1) {
//                struct object_st *temp = NULL;
//                struct object_st *obj = object_copy(state->temp_memory->top->data);
//                stack_pop(state->temp_memory);
//                if (obj->type == MAP_TYPE) {
//                    struct string_st *ind_str = string_new();
//
//                    string_set_str(ind_str, "__ne__", 6);
//
//                    sha256_code._code(ind_str, ind_str);
//                    temp = map_get_elm(obj->data, ind_str->data, ind_str->size);
//                    string_free(ind_str);
//                }
//
//                if (temp != NULL) {
//                    stack_add(state->temp_memory, obj);
//                    stack_add(state->temp_memory, temp);
//
//                    stack_add_new(code_operations, OP_BLOCK_TYPE);
//                    ((struct op_block *) code_operations->top->data->data)->type = BlockType_Call;
//                    ((struct op_block *) code_operations->top->data->data)->count = 1;
//                } else {
//                    struct object_st *res = object_new();
//
//                    object__neg(res, obj);
//
//                    stack_add(state->temp_memory, res);
//                    object_free(res);
//                }
//                object_free(temp);
//                object_free(obj);
//            }
//            break;
//        }
//        case BlockType_Put: {
//            stack_add(code_operations, block->data1);
//            stack_add(code_operations, block->data2);
//            break;
//        }
//        case BlockType_Delete_Temp: {
//            while (state->temp_memory->size > block->count) stack_pop(state->temp_memory);
//            break;
//        }
//        case BlockType_Delete_Scope: {
//            if (block->subtype == Delete_Scope_Func) {
//                if (state->return_obj == NULL) stack_add_new(state->temp_memory, NONE_TYPE);
//                else {
//                    stack_add(state->temp_memory, state->return_obj);
//                    object_free(state->return_obj);
//                    state->return_obj = NULL;
//                }
//            } else if (block->subtype == Delete_Scope_Class) {
//                object_set(state->temp_memory->top->data, state->memory->top->data);
//            }
//            if (state->memory->size > block->count) stack_pop(state->memory);
//
//            // TODO garbage collector
//            break;
//        }
//
//        case BlockType_Continue: {
//            struct object_st *current_object = NULL;
//            while (code_operations->size) {
//                current_object = code_operations->top->data;
//
//
//                if (current_object->type == OP_BLOCK_TYPE) {
//                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Temp &&
//                        state->temp_memory->size > ((struct op_block *) current_object->data)->count)
//                        stack_pop(state->temp_memory);
//                } else if (current_object->type == AST_NODE_TYPE) {
//                    if (((struct ast_node *) current_object->data)->type == StmtType_While) {
//                        break;
//                    }// TODO for and do while loop continue
//                }
//                stack_pop(code_operations);
//            }
//            break;
//        }
//        case BlockType_Break: {
//            struct object_st *current_object = NULL;
//            while (code_operations->size) {
//                current_object = code_operations->top->data;
//                if (current_object->type == OP_BLOCK_TYPE) {
//                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Temp &&
//                        state->temp_memory->size > ((struct op_block *) current_object->data)->count)
//                        stack_pop(state->temp_memory);
//                } else if (current_object->type == AST_NODE_TYPE) {
//                    if (((struct ast_node *) current_object->data)->type == StmtType_While) {
//                        stack_pop(code_operations);
//                        break;
//                    }// TODO for and do while loop break
//                }
//                stack_pop(code_operations);
//            }
//            break;
//        }
//        case BlockType_Return: {
//            if (block->count != 0 && state->return_obj == NULL) {
//                if (block->count == 1) {
//                    state->return_obj = object_copy(state->temp_memory->top->data);
//                    stack_pop(state->temp_memory);
//                } else {
//                    state->return_obj = object_new();
//                    object_set_type(state->return_obj, LIST_TYPE);
//                    for (size_t i = 0; i < block->count; i++) {
//                        list_append(state->return_obj->data, state->temp_memory->top->data);
//                        stack_pop(state->temp_memory);
//                    }
//                }
//            }
//            struct object_st *current_object = NULL;
//            while (code_operations->size) {
//                current_object = code_operations->top->data;
//                if (current_object->type == OP_BLOCK_TYPE) {
//                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Temp &&
//                        state->temp_memory->size > ((struct op_block *) current_object->data)->count)
//                        stack_pop(state->temp_memory);
//                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Scope) break;
//                }
//                stack_pop(code_operations);
//            }
//            break;
//        }
//
//        case BlockType_List: {
//            struct object_st *obj = object_new();
//            object_set_type(obj, LIST_TYPE);
//            for (size_t i = 0; i < block->count; i++) {
//                list_append(obj->data, state->temp_memory->top->data);
//                stack_pop(state->temp_memory);
//            }
//            stack_add(state->temp_memory, obj);
//            object_free(obj);
//            break;
//        }
//        case BlockType_Attr: {
//            struct object_st *res = NULL;
//            struct string_st *ind_str = ((struct ast_node *) block->data1->data)->data->data;
//            struct object_st *obj = object_copy(state->temp_memory->top->data);
//            if (code_operations->top->data->type != OP_BLOCK_TYPE ||
//                ((struct op_block *) code_operations->top->data->data)->type != BlockType_Call) {
//                stack_pop(state->temp_memory);
//            } else {
//                ((struct op_block *) code_operations->top->data->data)->count++;
//            }
//
//            if (obj->type == MAP_TYPE) {
//                res = map_set_elm(obj->data, ind_str->data, ind_str->size);
//                stack_add(state->temp_memory, res);
//            } else {
//                stack_add_new(state->temp_memory, NONE_TYPE);
//            }
//
//            object_free(res);
//            object_free(obj);
//            break;
//        }
//        case BlockType_Func: {
//            struct object_st *res = NULL;
//            struct string_st *ind_str = string_new();
//            struct object_st *obj = object_copy(state->temp_memory->top->data);
//
//            object_set_type(obj, MAP_TYPE);
//            if (obj->type == MAP_TYPE) {
//                string_set_str(ind_str, "__params__", 10);
//                sha256_code._code(ind_str, ind_str);
//                res = map_set_elm(obj->data, ind_str->data, ind_str->size);
//                object_set(res, block->data1);
//                object_free(res);
//            }
//
//            if (obj->type == MAP_TYPE) {
//                string_set_str(ind_str, "__call__", 7);
//                sha256_code._code(ind_str, ind_str);
//                res = map_set_elm(obj->data, ind_str->data, ind_str->size);
//                object_set(res, block->data2);
//                object_free(res);
//            }
//
//            string_free(ind_str);
//            object_free(obj);
//            break;
//        }
//        case BlockType_Call: {
//            struct object_st *res = NULL;
//            struct string_st *ind_str = string_new();
//            struct object_st *func = object_copy(state->temp_memory->top->data);
//            stack_pop(state->temp_memory);
//
//
//            {
//                stack_add_new(code_operations, OP_BLOCK_TYPE);
//                ((struct op_block *) code_operations->top->data->data)->type = BlockType_Delete_Scope;
//                ((struct op_block *) code_operations->top->data->data)->subtype = Delete_Scope_Func;
//                ((struct op_block *) code_operations->top->data->data)->count = state->memory->size;
//                stack_add_new(state->memory, MAP_TYPE);
//            }
//
//            if (func->type == MAP_TYPE) {
//                string_set_str(ind_str, "__init__", 8);
//                sha256_code._code(ind_str, ind_str);
//                res = map_get_elm(func->data, ind_str->data, ind_str->size);
//            }
//            int ok = 0;
//            if (res == NULL) {
//                struct string_st *ind_temp = NULL;
//                struct object_st *elm = NULL;
//                if (func->type == MAP_TYPE) {
//                    string_set_str(ind_str, "__params__", 10);
//                    sha256_code._code(ind_str, ind_str);
//                    res = map_set_elm(func->data, ind_str->data, ind_str->size);
//                }
//                if (res != NULL) {
//                    struct list_st *temp_list = ((struct ast_node *) res->data)->next;
//                    if (temp_list->size == block->count) {
//                        ok = 1;
//                        for (int i = 0; i < temp_list->size; i++) {
//                            ind_temp = ((struct ast_node *) temp_list->data[i]->data)->data->data;
//                            elm = map_set_elm(state->memory->top->data->data, ind_temp->data, ind_temp->size);
//                            object_set(elm, state->temp_memory->top->data);
//                            stack_pop(state->temp_memory);
//                            object_free(elm);
//                        }
//                    }
//                }
//                object_free(res);
//                if (func->type == MAP_TYPE && ok) {
//                    string_set_str(ind_str, "__call__", 7);
//                    sha256_code._code(ind_str, ind_str);
//                    res = map_set_elm(func->data, ind_str->data, ind_str->size);
//                    stack_add(code_operations, res);
//                }
//                object_free(res);
//            } else {
//                struct object_st *_res = NULL;
//                struct string_st *ind_temp = NULL;
//                struct object_st *elm = NULL;
//                if (res->type == MAP_TYPE) {
//                    string_set_str(ind_str, "__params__", 10);
//                    sha256_code._code(ind_str, ind_str);
//                    _res = map_set_elm(res->data, ind_str->data, ind_str->size);
//                }
//                if (res != NULL) {
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
//                            object_set(elm, state->temp_memory->top->data);
//                            stack_pop(state->temp_memory);
//                            object_free(elm);
//                        }
//                    }
//                }
//                object_free(_res);
//                _res = NULL;
//                if (res->type == MAP_TYPE && ok) {
//                    string_set_str(ind_str, "__call__", 7);
//                    sha256_code._code(ind_str, ind_str);
//                    _res = map_set_elm(res->data, ind_str->data, ind_str->size);
//                    stack_add(code_operations, _res);
//                }
//                object_free(_res);
//                object_free(res);
//
//            }
//
//            string_free(ind_str);
//            object_free(func);
//            break;
//        }
//        case BlockType_Class: {
//            stack_add_new(state->memory, MAP_TYPE);
//            break;
//        }
//
//    }
//}
//
//void run_smart_contract(struct op_state *state) {
//    struct stack_st *code_operations = state->code_operations;
//    struct object_st *current_object = NULL;
//    while (code_operations->size) {
//        current_object = object_copy(code_operations->top->data);
//        stack_pop(code_operations);
//
//        if (current_object->type == AST_NODE_TYPE) {
//            run_an(state, current_object);
//        }
//        if (current_object->type == OP_BLOCK_TYPE) {
//            run_op(state, current_object);
//        }
//        object_free(current_object);
//    }
//}
