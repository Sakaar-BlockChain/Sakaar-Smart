#include "sc_structs.h"

struct frame_st *frame_new() {
    struct frame_st *res = skr_malloc(sizeof (struct frame_st));
    attrib_list_data_init(&res->attrib);
    list_data_init(&res->data);

    res->counter = 1;
    return res;
}
void frame_set(struct frame_st *res, const struct frame_st *a) {
    attrib_list_set(&res->attrib, &a->attrib);
    list_set(&res->data, &a->data);
}
void frame_clear(struct frame_st *res) {
    attrib_list_clear(&res->attrib);
    list_clear(&res->data);
}
void frame_free(struct frame_st *res) {
    if(--res->counter > 0) return;
    attrib_list_data_free(&res->attrib);
    list_data_free(&res->data);
    skr_free(res);
}

struct frame_st *frame_copy(struct frame_st *res) {
    if (res == NULL) return NULL;
    res->counter ++;
    return res;
}
void frame_load_data(struct frame_st *res) {
    if(res == NULL) return;
    for(size_t i = 0; i < res->attrib.size; i++){
        if(res->attrib.attribs[i]->data != NULL) object_free(res->attrib.attribs[i]->data);
        res->attrib.attribs[i]->data = res->data.data[i];
        res->data.data[i] = NULL;
    }
}
void frame_save_data(struct frame_st *res, struct attrib_list_st *data) {
    attrib_list_set(&res->attrib, data);
    list_resize(&res->data, res->attrib.size);
    for(size_t i = 0; i < res->attrib.size; i++){
        if(res->data.data[i] != NULL) object_free(res->data.data[i]);
        res->data.data[i] = res->attrib.attribs[i]->data;
        res->attrib.attribs[i]->data = NULL;
    }
}