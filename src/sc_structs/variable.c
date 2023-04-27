#include "sc_structs.h"

struct variable_st *variable_new(size_t position) {
    struct variable_st *res = skr_malloc(sizeof(struct variable_st));
    string_data_init(&res->name);
    res->position = position;
    return res;
}
void variable_set(struct variable_st *res, const struct variable_st *a) {
    string_set(&res->name, &a->name);
    res->position = a->position;
}
void variable_clear(struct variable_st *res) {
    string_clear(&res->name);
    res->position = 0;
}
void variable_free(struct variable_st *res) {
    string_data_free(&res->name);
    skr_free(res);
}

void variable_data_init(struct variable_st *res) {
    string_data_init(&res->name);
    res->position = 0;
}
void variable_data_free(struct variable_st *res) {
    string_data_free(&res->name);
}