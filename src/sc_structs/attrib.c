#include "sc_structs.h"

struct attrib_st *attrib_new() {
    struct attrib_st *res = skr_malloc(sizeof(struct attrib_st));
    string_data_init(&res->name);
    res->data = NULL;
    res->counter = 1;
    return res;
}
void attrib_set(struct attrib_st *res, const struct attrib_st *a) {
    attrib_clear(res);

    string_set(&res->name, &a->name);
    if (a->data != NULL) res->data = object_copy(a->data);
}
void attrib_clear(struct attrib_st *res) {
    string_clear(&res->name);
    if (res->data != NULL) object_free(res->data);
    res->data = NULL;
}
void attrib_free(struct attrib_st *res) {
    if(--res->counter > 0) return;

    string_data_free(&res->name);
    if (res->data != NULL) object_free(res->data);
    skr_free(res);
}

struct attrib_st *attrib_copy(struct attrib_st *res) {
    if(res == NULL) return NULL;
    res->counter ++;
    return res;
}