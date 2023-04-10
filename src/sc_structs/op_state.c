#include "sc_structs.h"

struct op_state *op_state_new() {
    struct op_state *res = skr_malloc(sizeof(struct op_state));
    res->stack_memory = stack_new();
    res->temp_memory = stack_new();
    res->code_operations = stack_new();

    res->error_obj = NULL;
    res->return_obj = NULL;
    return res;
}
void op_state_set(struct op_state *res, const struct op_state *a) {
    stack_set(res->stack_memory, a->stack_memory);
    stack_set(res->temp_memory, a->temp_memory);
    stack_set(res->code_operations, a->code_operations);

    if(res->error_obj != NULL) object_free(res->error_obj);
    res->error_obj = NULL;
    if(res->return_obj != NULL) object_free(res->return_obj);
    res->return_obj = NULL;
    if(a->error_obj != NULL) res->error_obj = object_copy(a->error_obj);
    if(a->return_obj != NULL) res->return_obj = object_copy(a->return_obj);
}
void op_state_clear(struct op_state *res) {
    stack_clear(res->stack_memory);
    stack_clear(res->temp_memory);
    stack_clear(res->code_operations);

    if(res->error_obj != NULL) object_free(res->error_obj);
    res->error_obj = NULL;
    if(res->return_obj != NULL) object_free(res->return_obj);
    res->return_obj = NULL;
}
void op_state_free(struct op_state *res) {
    stack_free(res->stack_memory);
    stack_free(res->temp_memory);
    stack_free(res->code_operations);
    if(res->error_obj != NULL) object_free(res->error_obj);
    if(res->return_obj != NULL) object_free(res->return_obj);
    skr_free(res);
}

void op_state_append(struct op_state *res) {
    stack_add_new(res->stack_memory, LIST_TYPE);
}
void op_state_remove(struct op_state *res ) {
    stack_pop(res->stack_memory);
}