#include "sc_structs.h"


void attrib_list_set(struct attrib_list_st *res, const struct attrib_list_st *a) {
    attrib_list_resize(res, 0);
    attrib_list_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->attribs[i] = attrib_copy(a->attribs[i]);
}
void attrib_list_clear(struct attrib_list_st *res) {
    attrib_list_resize(res, 0);
}

void attrib_list_data_init(struct attrib_list_st *res) {
    res->attribs = NULL;
    res->max_size = res->size = 0;
}
void attrib_list_data_free(struct attrib_list_st *res) {
    attrib_list_resize(res, 0);
    if (res->attribs != NULL) skr_free(res->attribs);
}

void attrib_list_resize(struct attrib_list_st *res, size_t size) {
    if (res->attribs == NULL && size != 0) {
        res->max_size = size;
        res->attribs = skr_malloc(sizeof(struct attrib_st *) * size);
        for (size_t i = 0; i < size; i++) res->attribs[i] = NULL;
    } else if (res->max_size < size) {
        res->attribs = skr_realloc(res->attribs, sizeof(struct attrib_st *) * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->attribs[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->attribs[i] != NULL) attrib_free(res->attribs[i]);
            res->attribs[i] = NULL;
        }
    }
    res->size = size;
}
void attrib_list_append(struct attrib_list_st *res, struct attrib_st *data) {
    attrib_list_resize(res, res->size + 1);
    res->attribs[res->size - 1] = attrib_copy(data);
}
void attrib_list_add_new(struct attrib_list_st *res) {
    attrib_list_resize(res, res->size + 1);
    res->attribs[res->size - 1] = attrib_new();
}
struct attrib_st *attrib_list_last(struct attrib_list_st *res) {
    return res->attribs[res->size - 1];
}