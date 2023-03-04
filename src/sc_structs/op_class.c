#include "sc_structs.h"

struct object_type op_class_type = {OP_CLASS_OP, NULL, NULL};
struct object_type op_class_obj_type = {OP_CLASS_OBJ_OP, NULL, NULL};

struct op_class *op_class_new() {
    struct op_class *res = skr_malloc(OP_CLASS_SIZE);
    res->data = map_new();
    res->init = NULL;
    return res;
}
void op_class_set(struct op_class *res, const struct op_class *a) {
    op_class_clear(res);
    map_set(res->data, a->data);
    res->init = object_copy(a->init);
}
void op_class_clear(struct op_class *res) {
    map_clear(res->data);
    if (res->init != NULL) object_free(res->init);
    res->init = NULL;
}
void op_class_free(struct op_class *res) {
    map_free(res->data);
    if (res->init != NULL) object_free(res->init);
    skr_free(res);
}


struct op_class_obj *op_class_obj_new() {
    struct op_class_obj *res = skr_malloc(OP_CLASS_OBJ_SIZE);
    res->data = map_new();
    res->class = NULL;
    return res;
}
void op_class_obj_set(struct op_class_obj *res, const struct op_class_obj *a) {
    op_class_obj_clear(res);
    map_set(res->data, a->data);
    res->class = object_copy(a->class);
}
void op_class_obj_clear(struct op_class_obj *res) {
    map_clear(res->data);
    if (res->class != NULL) object_free(res->class);
    res->class = NULL;
}
void op_class_obj_free(struct op_class_obj *res) {
    map_free(res->data);
    if (res->class != NULL) object_free(res->class);
    skr_free(res);
}

void op_class_object_init(struct op_class_obj *res, struct object_st *class) {
    op_class_obj_clear(res);
    res->class = object_copy(class);
}