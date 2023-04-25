#include "sc_structs.h"

struct closure_st *closure_new() {
    struct closure_st *res = skr_malloc(sizeof(struct closure_st));
    variable_list_data_init(&res->attrib);
    variable_list_data_init(&res->data);
    return res;
}
void closure_set(struct closure_st *res, const struct closure_st *a) {
    variable_list_set(&res->attrib, &a->attrib);
    variable_list_set(&res->data, &a->data);
}
void closure_clear(struct closure_st *res) {
    variable_list_clear(&res->attrib);
    variable_list_clear(&res->data);
}
void closure_free(struct closure_st *res) {
    variable_list_data_free(&res->attrib);
    variable_list_data_free(&res->data);
    skr_free(res);
}

void closure_data_init(struct closure_st *res) {
    variable_list_data_init(&res->attrib);
    variable_list_data_init(&res->data);
}
void closure_data_free(struct closure_st *res) {
    variable_list_data_free(&res->attrib);
    variable_list_data_free(&res->data);
}

void closure_append(struct closure_st *res, struct variable_st *data, struct variable_st *ptr) {
    variable_list_append(&res->attrib, data);
    variable_list_append(&res->data, ptr);
}