#include "sc_structs.h"


void closure_list_set(struct closure_list_st *res, const struct closure_list_st *a) {
    if(res->type) return;
    closure_list_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->closures[i] = a->closures[i];
}
void closure_list_clear(struct closure_list_st *res) {
    closure_list_resize(res, 0);
}

void closure_list_data_init(struct closure_list_st *res) {
    res->closures = NULL;
    res->max_size = res->size = 0;
    res->type = 0;
}
void closure_list_data_free(struct closure_list_st *res) {
    closure_list_resize(res, 0);
    if (res->closures != NULL) skr_free(res->closures);
}

void closure_list_resize(struct closure_list_st *res, size_t size) {
    if (res->closures == NULL && size != 0) {
        res->max_size = size;
        res->closures = skr_malloc(sizeof(struct closure_st *) * size);
        for (size_t i = 0; i < size; i++) res->closures[i] = NULL;
    } else if (res->max_size < size) {
        res->closures = skr_realloc(res->closures, sizeof(struct closure_st *) * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->closures[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->type && res->closures[i] != NULL) closure_free(res->closures[i]);
            res->closures[i] = NULL;
        }
    }
    res->size = size;
}
void closure_list_append(struct closure_list_st *res, struct closure_st *data) {
    closure_list_resize(res, res->size + 1);
    res->closures[res->size - 1] = data;
}
void closure_list_add_new(struct closure_list_st *res) {
    if(!res->type) return;
    closure_list_resize(res, res->size + 1);
    res->closures[res->size - 1] = closure_new();
}
struct closure_st *closure_list_last(struct closure_list_st *res) {
    if (res->closures == NULL || res->size == 0) return NULL;
    return res->closures[res->size - 1];
}