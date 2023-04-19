#include "sc_structs.h"


void frame_list_set(struct frame_list_st *res, const struct frame_list_st *a) {
    frame_list_resize(res, 0);
    frame_list_resize(res, a->size);
    for (size_t i = 0; i < a->size; i++) res->frames[i] = frame_copy(a->frames[i]);
}
void frame_list_clear(struct frame_list_st *res) {
    frame_list_resize(res, 0);
}

void frame_list_data_init(struct frame_list_st *res) {
    res->frames = NULL;
    res->max_size = res->size = 0;
}
void frame_list_data_free(struct frame_list_st *res) {
    frame_list_resize(res, 0);
    if (res->frames != NULL) {
        for (size_t i = 0; i < res->max_size; i++) {
            frame_free(res->frames[i]);
        }
        skr_free(res->frames);
    }
}

void frame_list_resize(struct frame_list_st *res, size_t size) {
    if (res->frames == NULL && size != 0) {
        res->max_size = size;
        res->frames = skr_malloc(sizeof(struct frame_st *) * size);
        for (size_t i = 0; i < size; i++) res->frames[i] = frame_new();
    } else if (res->max_size < size) {
        res->frames = skr_realloc(res->frames, sizeof(struct frame_st *) * size * 2);
        for (size_t i = res->max_size; i < size * 2; i++) res->frames[i] = frame_new();
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            frame_clear(res->frames[i]);
        }
    }
    res->size = size;
}
void frame_list_append(struct frame_list_st *res, struct frame_st *data) {
    frame_list_resize(res, res->size + 1);
    res->frames[res->size - 1] = frame_copy(data);
}
void frame_list_add_new(struct frame_list_st *res) {
    frame_list_resize(res, res->size + 1);
}
struct frame_st *frame_list_last(struct frame_list_st *res) {
    if (res->frames == NULL || res->size == 0) return NULL;
    return res->frames[res->size - 1];
}