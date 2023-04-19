#include "op_structs.h"

struct object_type op_object_type = {OP_OBJECT_OP, NULL};

struct op_object *op_object_new() {
    struct op_object *res = skr_malloc(OP_OBJECT_SIZE);
    res->data = NULL;

    res->class = NULL;
    res->attr = map_new();

    res->closure = NULL;
    res->argument = NULL;
    res->function_body = NULL;

    res->func = NULL;

    return res;
}
void op_object_set(struct op_object *res, const struct op_object *a) {
    op_object_clear(res);

    res->data = object_copy(a->data);

    res->class = object_copy(a->class);
    map_set(res->attr, a->attr);

    res->closure = frame_copy(a->closure);
    res->argument = a->argument;
    res->function_body = a->function_body;

    res->func = a->func;
}
void op_object_clear(struct op_object *res) {
    if(res->data != NULL) object_free(res->data);
    res->data = NULL;

    if(res->class != NULL) object_free(res->class);
    res->class = NULL;
    map_clear(res->attr);

    res->closure = NULL;
    res->argument = NULL;
    res->function_body = NULL;

    res->func = NULL;
}
void op_object_free(struct op_object *res) {
    if(res->data != NULL) object_free(res->data);

    if(res->class != NULL) object_free(res->class);
    map_free(res->attr);

    skr_free(res);
}

void op_object_set_function
(struct op_object *res, struct variable_st *arg, struct closure_st *cls, struct block_expr_st *body) {
    res->argument = arg;
    res->function_body = body;

    res->closure = frame_new();
    attrib_list_set(&res->closure->attrib, &cls->attrib);
    list_resize(&res->closure->data, cls->data.size);
    for(size_t i = 0; i < cls->data.size; i++){
        if(res->closure->data.data[i] != NULL) object_free(res->closure->data.data[i]);
        res->closure->data.data[i] = object_copy(cls->data.attribs[i]->data);
    }
}
struct object_st *op_object_get_attrib(struct op_object *res, struct string_st *name) {
    return map_get_elm(res->attr, name->data, name->size);
}
struct object_st *op_object_set_attrib(struct op_object *res, struct string_st *name) {
    return map_set_elm(res->attr, name->data, name->size);
}