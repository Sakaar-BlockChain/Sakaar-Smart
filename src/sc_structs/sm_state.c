#include "sc_structs.h"

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
        if (op_attrib_cmp_name(list->data[i]->data, obj->data) == 0) {
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
            if (op_attrib_cmp_name(list->data[j]->data, obj->data) == 0) {
                ptr = object_copy(list->data[j]);
                break;
            }
        }
        if (ptr != NULL) break;
    }

    if (ptr == NULL) return NULL;

    for (; i < res->memory_frame->size; i++) {
        if (res->memory_closure->data[i] == NULL) continue;
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