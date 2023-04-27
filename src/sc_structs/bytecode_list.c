#include "sc_structs.h"

void bytecode_list_set(struct bytecode_list_st *res, const struct bytecode_list_st *a) {
    if(res->type) return;
    bytecode_list_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->bytecodes[i] = a->bytecodes[i];
}
void bytecode_list_clear(struct bytecode_list_st *res) {
    bytecode_list_resize(res, 0);
}

void bytecode_list_data_init(struct bytecode_list_st *res) {
    res->bytecodes = NULL;
    res->max_size = 0;
    res->size = 0;
    res->type = 0;
}
void bytecode_list_data_free(struct bytecode_list_st *res) {
    bytecode_list_resize(res, 0);
    if (res->bytecodes != NULL) skr_free(res->bytecodes);
}

void bytecode_list_resize(struct bytecode_list_st *res, size_t size) {
    if (res->bytecodes == NULL && size != 0) {
        res->max_size = size;
        res->bytecodes = skr_malloc(sizeof(struct bytecode_st *) * size);
        for (size_t i = 0; i < size; i++) res->bytecodes[i] = NULL;
    } else if (res->max_size < size) {
        res->bytecodes = skr_realloc(res->bytecodes, sizeof(struct bytecode_st *) * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->bytecodes[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->type && res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->bytecodes[i] != NULL) bytecode_free(res->bytecodes[i]);
            res->bytecodes[i] = NULL;
        }
    }
    res->size = size;
}
void bytecode_list_append(struct bytecode_list_st *res, struct bytecode_st *data) {
    bytecode_list_resize(res, res->size + 1);
    res->bytecodes[res->size - 1] = data;
}
void bytecode_list_add_new(struct bytecode_list_st *res) {
    if(!res->type) return;
    bytecode_list_resize(res, res->size + 1);
    res->bytecodes[res->size - 1] = bytecode_new();
}
struct bytecode_st *bytecode_list_last(struct bytecode_list_st *res) {
    if (res->bytecodes == NULL || res->size == 0) return NULL;
    return res->bytecodes[res->size - 1];
}
struct bytecode_st *bytecode_list_pop(struct bytecode_list_st *res) {
    if (res->type || res->bytecodes == NULL || res->size == 0) return NULL;
    return res->bytecodes[--res->size];
}