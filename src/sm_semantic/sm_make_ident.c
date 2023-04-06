#include <stdio.h>
#include "sm_semantic.h"

#define ScopeType_None  0x00
#define ScopeType_Func  0x01
#define ScopeType_Class 0x02
#define ScopeType_Loop  0x04


struct sm_state {
    struct list_st *memory_list;

    struct stack_st *memory_stack;
    struct stack_st *expr_stack;

    int type;
    int error;
};

struct sm_state *sm_state_new() {
    struct sm_state *res = skr_malloc(sizeof(struct sm_state));
    res->type = ScopeType_None;
    res->error = 0;

    res->memory_list = list_new();

    res->memory_stack = stack_new();
    res->expr_stack = stack_new();
    return res;
}

void sm_state_free(struct sm_state *res) {
    list_free(res->memory_list);
    stack_free(res->memory_stack);
    stack_free(res->expr_stack);
    skr_free(res);
}

void sm_state_save_type(struct sm_state *res) {
    stack_add_new(res->expr_stack, INTEGER_TYPE);
    integer_set_si(res->expr_stack->top->data->data, res->type);

    stack_add_new(res->memory_stack, INTEGER_TYPE);
    integer_set_ui(res->memory_stack->top->data->data, res->memory_list->size);
}

size_t sm_state_add(struct sm_state *res, struct object_st *obj){
    list_append(res->memory_list, obj);
    return res->memory_list->size;
}
size_t sm_state_find(struct sm_state *res, struct object_st *obj){
    for(size_t i = res->memory_list->size; i > 0; i--){
        if(object_cmp(res->memory_list->data[i - 1], obj) == 0) return i;
    }
    return 0;
}

void semantic_scan_fields(struct sm_state *state, struct object_st *obj) {
    struct ast_node *node = obj->data;

    if (node->main_type == MainType_Expr) {
        switch (node->type) {
            case PrimType_Ident_get:{
                node->memory_pos = sm_state_find(state, node->data);
                if(node->memory_pos == 0) {
                    state->error = -1;
                    return;
                }
                return;
            }
            case PrimType_Ident_new: {
                node->memory_pos = sm_state_add(state, node->data);
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
                if (((state->type) & ScopeType_Class) != ScopeType_Class) {
                    state->error = -1;
                    return;
                }
                break;
            case StmtType_PUB_Func:
            case StmtType_STC_Func:
            case StmtType_PRI_Func:
                if (((state->type) & ScopeType_Class) != ScopeType_Class) {
                    state->error = -1;
                    return;
                }
            case StmtType_Func:
                sm_state_save_type(state);
                state->type = ScopeType_Func;
                break;
            case StmtType_Class:
                sm_state_save_type(state);
                state->type |= ScopeType_Class;
                break;
            case StmtType_While:
            case StmtType_DoWhile:
                sm_state_save_type(state);
                state->type |= ScopeType_Loop;
                break;
            case StmtType_List:
                sm_state_save_type(state);
                break;
        }
    }

    for (size_t i=0;i<node->next->size;i++){
        stack_add(state->expr_stack, node->next->data[node->next->size - i - 1]);
    }

}

int semantic_scan(struct object_st *expr_obj) {
    struct sm_state *state = sm_state_new();
    struct object_st *obj;

    stack_add(state->expr_stack, expr_obj);
    while (state->expr_stack->size) {
        obj = object_copy(state->expr_stack->top->data);
        stack_pop(state->expr_stack);

        if (obj->type == AST_NODE_TYPE) {
            semantic_scan_fields(state, obj);
            if (state->error != 0) return state->error;
        } else if (obj->type == INTEGER_TYPE) {
            state->type = integer_get_si(obj->data);
            object_free(obj);

            obj = object_copy(state->memory_stack->top->data);
            stack_pop(state->memory_stack);

            list_resize(state->memory_list, integer_get_ui(obj->data));
        }

        object_free(obj);
    }
    sm_state_free(state);
    return 0;
}