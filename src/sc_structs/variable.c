#include "sc_structs.h"

struct variable_st *variable_new() {
    struct variable_st *res = skr_malloc(sizeof(struct variable_st));
    string_data_init(&res->name);
    return res;
}
void variable_set(struct variable_st *res, const struct variable_st *a) {
    string_set(&res->name, &a->name);
}
void variable_clear(struct variable_st *res) {
    string_clear(&res->name);
}
void variable_free(struct variable_st *res) {
    string_data_free(&res->name);
    skr_free(res);
}

void variable_data_init(struct variable_st *res) {
    string_data_init(&res->name);
}
void variable_data_free(struct variable_st *res) {
    string_data_free(&res->name);
}