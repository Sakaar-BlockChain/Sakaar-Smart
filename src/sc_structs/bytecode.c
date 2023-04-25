#include "sc_structs.h"

struct bytecode_st *bytecode_new() {
    struct bytecode_st *res = skr_malloc(sizeof (struct bytecode_st));
    res->data = NULL;
    res->command = NULL;

    res->size = res->max_size = 0;
    return res;
}
void bytecode_free(struct bytecode_st *res) {
    if(res->data != NULL) skr_free(res->data);
    if(res->command != NULL) skr_free(res->command);
    skr_free(res);
}

void bytecode_resize(struct bytecode_st *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = skr_malloc(size * sizeof(void *));
        res->command = skr_malloc(size);
        if (res->data != NULL) for (size_t i = 0; i < size; i++) res->data[i] = NULL;
        if (res->command != NULL) for (size_t i = 0; i < size; i++) res->command[i] = 0;
    } else if (res->max_size < size) {
        res->data = skr_realloc(res->data, size * 2 * sizeof(void *));
        res->command = skr_realloc(res->command, size * 2);
        if (res->data != NULL) for (size_t i = res->max_size; i < size * 2; i++) res->data[i] = NULL;
        if (res->command != NULL) for (size_t i = res->max_size; i < size * 2; i++) res->command[i] = 0;
        res->max_size = size * 2;
    }
    if (res->size > size)
        for (size_t i = size; i < res->size; i++) {
            res->data[i] = NULL;
            res->command[i] = 0;
        }
    res->size = size;
}
void bytecode_append(struct bytecode_st *res, char command, void *data) {
    bytecode_resize(res, res->size + 1);
    res->command[res->size - 1] = command;
    res->data[res->size - 1] = data;
}