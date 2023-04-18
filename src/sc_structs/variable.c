#include "sc_structs.h"

struct variable_st *variable_new() {
    struct variable_st *res = skr_malloc(sizeof(struct variable_st));
    attrib_list_data_init(&res->attrib);
    return res;
}
void variable_set(struct variable_st *res, const struct variable_st *a) {
    attrib_list_set(&res->attrib, &a->attrib);
}
void variable_clear(struct variable_st *res) {
    attrib_list_clear(&res->attrib);
}
void variable_free(struct variable_st *res) {
    attrib_list_data_free(&res->attrib);
    skr_free(res);
}

void variable_data_init(struct variable_st *res) {
    attrib_list_data_init(&res->attrib);
}
void variable_data_free(struct variable_st *res) {
    attrib_list_data_free(&res->attrib);
}