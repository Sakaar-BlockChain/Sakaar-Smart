#include "sc_structs.h"

struct op_state *op_state_new() {
    struct op_state *res = skr_malloc(sizeof(struct op_state));
    res->stack_memory = list_new();
    res->frame_memory = NULL;
    res->temp_memory = list_new();
    res->code_operations = list_new();

    res->error_obj = NULL;
    res->return_obj = NULL;
    return res;
}
void op_state_free(struct op_state *res) {
    list_free(res->stack_memory);

    if(res->frame_memory != NULL) object_free(res->frame_memory);
    list_free(res->temp_memory);
    list_free(res->code_operations);
    if(res->error_obj != NULL) object_free(res->error_obj);
    if(res->return_obj != NULL) object_free(res->return_obj);
    skr_free(res);
}