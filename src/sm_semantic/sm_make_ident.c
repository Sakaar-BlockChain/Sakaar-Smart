#include <stdio.h>
#include "sm_semantic.h"

#define ScopeType_None  0x00
#define ScopeType_Func  0x01
#define ScopeType_Class 0x02
#define ScopeType_Loop  0x04


struct sm_state {
    struct list_st *memory_frame;
    struct list_st *memory_closure;
    struct list_st *expr_stack;

    int type;
    int error;
};

struct sm_state *sm_state_new() {
    struct sm_state *res = skr_malloc(sizeof(struct sm_state));
    res->type = ScopeType_None;
    res->error = 0;

    res->memory_frame = list_new();
    res->memory_closure = list_new();
    res->expr_stack = list_new();
    return res;
}

void sm_state_free(struct sm_state *res) {
    list_free(res->memory_frame);
    list_free(res->memory_closure);
    list_free(res->expr_stack);
    skr_free(res);
}

void sm_state_save_type(struct sm_state *res, struct ast_node *obj) {
    list_add_new(res->expr_stack, INTEGER_TYPE);
    integer_set_si(list_get_last(res->expr_stack)->data, res->type);

    list_add_new(res->memory_frame, LIST_TYPE);
    list_append(res->memory_closure, obj->closure);
}

void sm_state_remove_frame(struct sm_state *res) {
    list_remove_last(res->memory_frame);
    list_remove_last(res->memory_closure);
}

struct object_st *sm_state_set_ident(struct sm_state *res, struct object_st *obj) {
    struct list_st *list = list_get_last(res->memory_frame)->data;
    for (size_t i = 0; i < list->size; i++) {
        if(op_attrib_cmp_name(list->data[i]->data, obj->data) == 0)  {
            return object_copy(list->data[i]);
        }
    }
    list_add_new(list, OP_ATTRIB_TYPE);
    op_attrib_set_name(list_get_last(list)->data, obj->data);
    return object_copy(list_get_last(list));
}

struct object_st *sm_state_get_ident(struct sm_state *res, struct object_st *obj) {
    struct list_st *list;
    struct dlist_st *closure;

    size_t i = res->memory_frame->size;
    struct object_st *ptr = NULL;

    for (; i > 0; i--) {
        list = res->memory_frame->data[i - 1]->data;
        for (size_t j = 0; j < list->size; j++) {
            if(op_attrib_cmp_name(list->data[j]->data, obj->data) == 0) {
                ptr = object_copy(list->data[j]);
                break;
            }
        }
        if(ptr != NULL) break;
    }

    if(ptr == NULL) return NULL;

    for (; i < res->memory_frame->size; i++){
        if(res->memory_closure->data[i] == NULL) continue;
        list = res->memory_frame->data[i]->data;
        closure = res->memory_closure->data[i]->data;

        struct object_st *new_obj = object_new();
        object_set_type(new_obj, OP_ATTRIB_TYPE);
        op_attrib_set_name(new_obj->data, obj->data);

        dlist_append(closure, new_obj, ptr);
        list_append(list, new_obj);

        object_free(ptr);
        ptr = new_obj;
    }

    return ptr;
}

void semantic_scan_fields(struct sm_state *state, struct object_st *obj) {
    struct ast_node *node = obj->data;

    if (node->main_type == MainType_Expr) {
        switch (node->type) {
            case PrimType_Ident_get: {
                struct object_st *res = sm_state_get_ident(state, node->data);
                if (res == NULL) {
                    state->error = -1;
                    return;
                }
                object_free(node->data);
                node->data = res;
                return;
            }
            case PrimType_Ident_new: {
                struct object_st *res = sm_state_set_ident(state, node->data);
                object_free(node->data);
                node->data = res;
                return;
            }
        }
    }
    if (node->main_type == MainType_Stmt) {
        switch (node->type) {
            case StmtType_Params:
            case StmtType_Return:
                if (((state->type) & ScopeType_Func) != ScopeType_Func) {
                    state->error = -1;
                    return;
                }
                break;
            case StmtType_Break:
            case StmtType_Continue:
                if (((state->type) & ScopeType_Loop) != ScopeType_Loop) {
                    state->error = -1;
                    return;
                }
                break;
            case StmtType_Extends:
            case StmtType_PUB_Func:
            case StmtType_STC_Func:
            case StmtType_PRI_Func:
                if (((state->type) & ScopeType_Class) != ScopeType_Class) {
                    state->error = -1;
                    return;
                }
                break;
            case StmtType_Func_Body:
                node->closure = object_new();
                object_set_type(node->closure, DLIST_TYPE);
                sm_state_save_type(state, node);
                state->type = ScopeType_Func;
                break;
            case StmtType_Class:
                sm_state_save_type(state, node);
                state->type |= ScopeType_Class;
                break;
            case StmtType_While:
            case StmtType_DoWhile:
                sm_state_save_type(state, node);
                state->type |= ScopeType_Loop;
                break;
            case StmtType_List:
                sm_state_save_type(state, node);
                break;
        }
    }

    for (size_t i = 0; i < node->next->size; i++) {
        list_append(state->expr_stack, node->next->data[node->next->size - i - 1]);
    }

}

int semantic_scan(struct object_st *expr_obj) {
    struct sm_state *state = sm_state_new();
    struct object_st *obj;

    list_append(state->expr_stack, expr_obj);
    while (state->expr_stack->size && state->error == 0) {
        obj = object_copy(list_get_last(state->expr_stack));
        list_remove_last(state->expr_stack);

        if (obj->type == AST_NODE_TYPE) {
            semantic_scan_fields(state, obj);
        } else if (obj->type == INTEGER_TYPE) {
            state->type = integer_get_si(obj->data);
            sm_state_remove_frame(state);
        }

        object_free(obj);
    }
    sm_state_free(state);
    return state->error;
}