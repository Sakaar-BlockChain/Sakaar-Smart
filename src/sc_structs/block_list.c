#include "sc_structs.h"


void block_list_set(struct block_list_st *res, const struct block_list_st *a) {
    if(res->type) return;
    block_list_resize(res, a->size);

    for (size_t i = 0; i < a->size; i++) {
        res->blocks[i] = a->blocks[i];
        res->blocks_pos[i] = a->blocks_pos[i];
    }
}
void block_list_clear(struct block_list_st *res) {
    block_list_resize(res, 0);
}

void block_list_data_init(struct block_list_st *res) {
    res->blocks = NULL;
    res->blocks_pos = NULL;
    res->max_size = res->size = 0;
    res->type = 0;
}
void block_list_data_free(struct block_list_st *res) {
    block_list_resize(res, 0);
    if (res->blocks != NULL) skr_free(res->blocks);
    if (res->blocks_pos != NULL) skr_free(res->blocks_pos);
}

void block_list_resize(struct block_list_st *res, size_t size) {
    if (res->blocks == NULL && size != 0) {
        res->max_size = size;
        res->blocks = skr_malloc(sizeof(struct block_expr_st *) * size);
        res->blocks_pos = skr_malloc(sizeof(size_t) * size);
        for (size_t i = 0; i < size; i++) res->blocks[i] = NULL;
        for (size_t i = 0; i < size; i++) res->blocks_pos[i] = 0;
    } else if (res->max_size < size) {
        res->blocks = skr_realloc(res->blocks, sizeof(struct block_expr_st *) * size * 2);
        res->blocks_pos = skr_realloc(res->blocks_pos, sizeof(size_t) * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->blocks[i] = NULL;
        for (size_t i = res->max_size; i < size * 2; i++) res->blocks_pos[i] = 0;
        res->max_size = size * 2;
    }
    if (res->type && res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->type && res->blocks[i] != NULL) block_expr_free(res->blocks[i]);
            res->blocks[i] = NULL;
            res->blocks_pos[i] = 0;
        }
    }
    res->size = size;
}
void block_list_append(struct block_list_st *res, struct block_expr_st *data, size_t position) {
    block_list_resize(res, res->size + 1);
    res->blocks[res->size - 1] = data;
    res->blocks_pos[res->size - 1] = position;
}
void block_list_add_new(struct block_list_st *res) {
    if(!res->type) return;
    block_list_resize(res, res->size + 1);
    res->blocks[res->size - 1] = block_expr_new();
    res->blocks_pos[res->size - 1] = 0;
}
struct block_expr_st *block_list_last(struct block_list_st *res) {
    if (res->blocks == NULL || res->size == 0) return NULL;
    return res->blocks[res->size - 1];
}
size_t block_list_last_pos(struct block_list_st *res) {
    if (res->blocks_pos == NULL || res->size == 0) return 0;
    return res->blocks_pos[res->size - 1];
}