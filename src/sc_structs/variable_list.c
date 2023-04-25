#include "sc_structs.h"


struct variable_list_st *variable_list_new() {
    struct variable_list_st *res = skr_malloc(sizeof(struct variable_list_st));
    res->variables = NULL;
    res->max_size = res->size = 0;
    res->type = 0;
    return res;
}
void variable_list_set(struct variable_list_st *res, const struct variable_list_st *a) {
    if(res->type) return;
    variable_list_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->variables[i] = a->variables[i];
}
void variable_list_clear(struct variable_list_st *res) {
    variable_list_resize(res, 0);
}
void variable_list_free(struct variable_list_st *res) {
    variable_list_resize(res, 0);
    if (res->variables != NULL) skr_free(res->variables);
    skr_free(res);
}

void variable_list_data_init(struct variable_list_st *res) {
    res->variables = NULL;
    res->max_size = res->size = 0;
    res->type = 0;
}
void variable_list_data_free(struct variable_list_st *res) {
    variable_list_resize(res, 0);
    if (res->variables != NULL) skr_free(res->variables);
}

void variable_list_resize(struct variable_list_st *res, size_t size) {
    if (res->variables == NULL && size != 0) {
        res->max_size = size;
        res->variables = skr_malloc(sizeof(struct variable_st *) * size);
        for (size_t i = 0; i < size; i++) res->variables[i] = NULL;
    } else if (res->max_size < size) {
        res->variables = skr_realloc(res->variables, sizeof(struct variable_st *) * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->variables[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->type && res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->variables[i] != NULL) variable_free(res->variables[i]);
            res->variables[i] = NULL;
        }
    }
    res->size = size;
}
void variable_list_append(struct variable_list_st *res, struct variable_st *data) {
    variable_list_resize(res, res->size + 1);
    res->variables[res->size - 1] = data;
}
void variable_list_add_new(struct variable_list_st *res) {
    if(!res->type) return;
    variable_list_resize(res, res->size + 1);
    res->variables[res->size - 1] = variable_new();
}
struct variable_st *variable_list_last(struct variable_list_st *res) {
    if (res->variables == NULL || res->size == 0) return NULL;
    return res->variables[res->size - 1];
}