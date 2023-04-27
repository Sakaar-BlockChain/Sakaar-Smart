#include "sc_structs.h"


void variable_list_list_set(struct variable_list_list_st *res, const struct variable_list_list_st *a) {
    if(res->type) return;
    variable_list_list_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->variable_lists[i] = a->variable_lists[i];
}
void variable_list_list_clear(struct variable_list_list_st *res) {
    variable_list_list_resize(res, 0);
}

void variable_list_list_data_init(struct variable_list_list_st *res) {
    res->variable_lists = NULL;
    res->max_size = 0;
    res->size = 0;
    res->type = 0;
}
void variable_list_list_data_free(struct variable_list_list_st *res) {
    variable_list_list_resize(res, 0);
    if (res->variable_lists != NULL) skr_free(res->variable_lists);
}

void variable_list_list_resize(struct variable_list_list_st *res, size_t size) {
    if (res->variable_lists == NULL && size != 0) {
        res->max_size = size;
        res->variable_lists = skr_malloc(sizeof(struct variable_list_st *) * size);
        for (size_t i = 0; i < size; i++) res->variable_lists[i] = NULL;
    } else if (res->max_size < size) {
        res->variable_lists = skr_realloc(res->variable_lists, sizeof(struct variable_list_st *) * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->variable_lists[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->type && res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->variable_lists[i] != NULL) variable_list_free(res->variable_lists[i]);
            res->variable_lists[i] = NULL;
        }
    }
    res->size = size;
}
void variable_list_list_append(struct variable_list_list_st *res, struct variable_list_st *data) {
    variable_list_list_resize(res, res->size + 1);
    res->variable_lists[res->size - 1] = data;
}
void variable_list_list_add_new(struct variable_list_list_st *res) {
    if(!res->type) return;
    variable_list_list_resize(res, res->size + 1);
    res->variable_lists[res->size - 1] = variable_list_new();
    res->variable_lists[res->size - 1]->type = res->type;
}
struct variable_list_st *variable_list_list_last(struct variable_list_list_st *res) {
    if (res->variable_lists == NULL || res->size == 0) return NULL;
    return res->variable_lists[res->size - 1];
}