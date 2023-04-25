#include "sc_structs.h"

struct frame_st *frame_new() {
    struct frame_st *res = skr_malloc(sizeof(struct frame_st));
    variable_list_data_init(&res->attrib);
    list_data_init(&res->data);
    return res;
}
void frame_set(struct frame_st *res, const struct frame_st *a) {
    variable_list_set(&res->attrib, &a->attrib);
    list_set(&res->data, &a->data);
}
void frame_clear(struct frame_st *res) {
    variable_list_clear(&res->attrib);
    list_clear(&res->data);
}
void frame_free(struct frame_st *res) {
    variable_list_data_free(&res->attrib);
    list_data_free(&res->data);
    skr_free(res);
}