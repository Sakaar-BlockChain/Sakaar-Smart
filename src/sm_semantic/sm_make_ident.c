//#include <stdio.h>
//#include "sm_semantic.h"
//
//
//void semantic_scan_fields(struct sm_state *state, struct object_st *obj) {
//    struct ast_node *node = obj->data;
//
//    if (node->main_type == MainType_Expr) {
//        switch (node->type) {
//            case PrimType_Ident_get: {
//                struct object_st *res = sm_state_get_ident(state, node->data);
//                if (res == NULL) {
//                    state->error = -1;
//                    return;
//                }
//                object_free(node->data);
//                node->data = res;
//                return;
//            }
//            case PrimType_Ident_new: {
//                struct object_st *res = sm_state_set_ident(state, node->data);
//                object_free(node->data);
//                node->data = res;
//                return;
//            }
//        }
//    }
//    if (node->main_type == MainType_Stmt) {
//        switch (node->type) {
//            case StmtType_Params:
//            case StmtType_Return:
//                if (((state->type) & ScopeType_Func) != ScopeType_Func) {
//                    state->error = -1;
//                    return;
//                }
//                break;
//            case StmtType_Break:
//            case StmtType_Continue:
//                if (((state->type) & ScopeType_Loop) != ScopeType_Loop) {
//                    state->error = -1;
//                    return;
//                }
//                break;
//            case StmtType_Extends:
//            case StmtType_PUB_Func:
//            case StmtType_STC_Func:
//            case StmtType_PRI_Func:
//                if (((state->type) & ScopeType_Class) != ScopeType_Class) {
//                    state->error = -1;
//                    return;
//                }
//                break;
//            case StmtType_Func_Body:
//                node->closure = object_new();
//                object_set_type(node->closure, DLIST_TYPE);
//                sm_state_save_type(state, node);
//                state->type = ScopeType_Func;
//                break;
//            case StmtType_Class:
//                sm_state_save_type(state, node);
//                state->type |= ScopeType_Class;
//                break;
//            case StmtType_While:
//            case StmtType_DoWhile:
//                sm_state_save_type(state, node);
//                state->type |= ScopeType_Loop;
//                break;
//            case StmtType_List:
//                sm_state_save_type(state, node);
//                break;
//        }
//    }
//
//    for (size_t i = 0; i < node->next->size; i++) {
//        list_append(state->expr_stack, node->next->data[node->next->size - i - 1]);
//    }
//
//}
//
//int semantic_scan(struct object_st *expr_obj) {
//    struct sm_state *state = sm_state_new();
//    struct object_st *obj;
//
//    list_append(state->expr_stack, expr_obj);
//    while (state->expr_stack->size && state->error == 0) {
//        obj = object_copy(list_get_last(state->expr_stack));
//        list_remove_last(state->expr_stack);
//
//        if (obj->type == AST_NODE_TYPE) {
//            semantic_scan_fields(state, obj);
//        } else if (obj->type == INTEGER_TYPE) {
//            state->type = integer_get_si(obj->data);
//            sm_state_remove_frame(state);
//        }
//
//        object_free(obj);
//    }
//    int error = state->error;
//    sm_state_free(state);
//    return error;
//}