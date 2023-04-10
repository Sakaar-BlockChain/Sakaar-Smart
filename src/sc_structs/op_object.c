#include "sc_structs.h"

struct object_type op_object_type = {OP_OBJECT_OP, NULL, NULL};

struct op_object *op_object_new() {
    struct op_object *res = skr_malloc(OP_OBJECT_SIZE);
    res->name = string_new();
    res->data = NULL;

    res->class = NULL;
    res->attr = list_new();

    res->args = list_new();
    res->body = NULL;

    res->closure = op_closure_new();

    return res;
}
void op_object_set(struct op_object *res, const struct op_object *a) {
    op_object_clear(res);

    string_set(res->name, a->name);
    res->data = object_copy(a->data);

    res->class = object_copy(a->class);
    list_set(res->attr, a->attr);

    list_set(res->args, a->args);
    res->body = object_copy(a->body);

    op_closure_set(res->closure, a->closure);
}
void op_object_clear(struct op_object *res) {
    string_clear(res->name);
    if(res->data != NULL) object_free(res->data);
    res->data = NULL;

    if(res->class != NULL) object_free(res->class);
    res->class = NULL;
    list_clear(res->attr);

    list_clear(res->args);
    if(res->body != NULL) object_free(res->body);
    res->body = NULL;

    op_closure_clear(res->closure);
}
void op_object_free(struct op_object *res) {
    string_free(res->name);
    if(res->data != NULL) object_free(res->data);

    if(res->class != NULL) object_free(res->class);
    list_free(res->attr);

    list_free(res->args);
    if(res->body != NULL) object_free(res->body);

    op_closure_free(res->closure);
    skr_free(res);
}


void op_object_set_name(struct op_object *res, const struct string_st *name) {
    string_set(res->name, name);
}
void op_object_set_data(struct op_object *res, struct object_st *obj) {
    if(res->data != NULL) object_free(res->data);
    res->data = object_copy(obj);
}