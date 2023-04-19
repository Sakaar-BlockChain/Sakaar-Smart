#include "sc_structs.h"


void node_list_set(struct node_list_st *res, const struct node_list_st *a) {
    if(res->type) return;
    node_list_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->nodes[i] = a->nodes[i];
}
void node_list_clear(struct node_list_st *res) {
    node_list_resize(res, 0);
}

void node_list_data_init(struct node_list_st *res) {
    res->nodes = NULL;
    res->max_size = res->size = 0;
    res->type = 0;
}
void node_list_data_free(struct node_list_st *res) {
    node_list_resize(res, 0);
    if (res->nodes != NULL) skr_free(res->nodes);
}

void node_list_resize(struct node_list_st *res, size_t size) {
    if (res->nodes == NULL && size != 0) {
        res->max_size = size;
        res->nodes = skr_malloc(sizeof(struct node_st *) * size);
        for (size_t i = 0; i < size; i++) res->nodes[i] = NULL;
    } else if (res->max_size < size) {
        res->nodes = skr_realloc(res->nodes, sizeof(struct node_st *) * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->nodes[i] = NULL;
        res->max_size = size * 2;
    }
    if (res->type && res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            if (res->nodes[i] != NULL) node_free(res->nodes[i]);
            res->nodes[i] = NULL;
        }
    }
    res->size = size;
}
void node_list_append(struct node_list_st *res, struct node_st *data) {
    node_list_resize(res, res->size + 1);
    res->nodes[res->size - 1] = data;
}
void node_list_add_new(struct node_list_st *res) {
    if(!res->type) return;
    node_list_resize(res, res->size + 1);
    res->nodes[res->size - 1] = node_new();
}
struct node_st *node_list_last(struct node_list_st *res) {
    if (res->nodes == NULL || res->size == 0) return NULL;
    return res->nodes[res->size - 1];
}