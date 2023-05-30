#include "op_structs.h"
struct object_sub op_object_sub = {METHOD_SUBSCRIPT &op_object_subscript, METHOD_ATTRIB &op_object_attrib};
//struct object_math_op op_object_math = {METHOD_MATH &op_object__mod, METHOD_MATH &op_object__and, METHOD_MATH &op_object__mul, METHOD_MATH &op_object__add, METHOD_MATH &op_object__sub, METHOD_MATH &op_object__div, METHOD_MATH &op_object__xor, METHOD_MATH &op_object__or, METHOD_MATH &op_object__ls, METHOD_MATH &op_object__rs, METHOD_CONVERT &op_object__neg};
//struct object_convert op_object_convert = {METHOD_CONVERT &op_object__bool, METHOD_CONVERT &op_object__int, METHOD_CONVERT &op_object__float, METHOD_CONVERT &op_object__str};
//struct object_type op_object_type = {OP_OBJECT_OP, NULL, &op_object_sub, &op_object_convert, &op_object_math};
struct object_type op_object_type = {OP_OBJECT_OP, NULL, &op_object_sub};

struct op_object *op_object_new() {
    struct op_object *res = skr_malloc(OP_OBJECT_SIZE);
    res->attr = map_new();

    res->closure = NULL;
    res->argument = NULL;
    res->class_body = 0;

    return res;
}
void op_object_free(struct op_object *res) {
    if (res == NULL) return;
    if(res->closure != NULL) frame_free(res->closure);
    map_free(res->attr);

    skr_free(res);
}

void op_object_set(struct op_object *res, const struct op_object *a) {
    if (res == NULL) return;
    op_object_clear(res);
    if (a == NULL) return;
    map_set(res->attr, a->attr);

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->class_body = a->class_body;
}
void op_object_copy(struct op_object *res, const struct op_object *a) {
    if (res == NULL) return;
    op_object_clear(res);
    if (a == NULL) return;
    map_copy(res->attr, a->attr);

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->class_body = a->class_body;
}

void op_object_clear(struct op_object *res) {
    if (res == NULL) return;
    map_clear(res->attr);

    if(res->closure != NULL) frame_free(res->closure);
    res->closure = NULL;
    res->argument = NULL;
    res->class_body = 0;
}
int op_object_cmp(const struct op_object *obj1, const struct op_object *obj2) {
    if (obj1 == NULL || obj2 == NULL || obj1->class_body != obj2->class_body) return CMP_NEQ;
    return CMP_EQ;
}

void op_object_define(struct op_object *res, struct op_class *class) {
//    map_copy(res->attr, class->attr);


    if (class->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, class->closure);
    }
    res->argument = class->argument;
    res->class_body = class->class_body;
}

// Sub method
struct object_st *op_object_subscript(struct error_st *err, struct op_object *obj, const struct object_st *obj2) {
    return map_subscript(err, obj->attr, obj2);
}
struct object_st *op_object_attrib(struct error_st *err, const struct op_object *obj, const struct string_st *str) {
    return map_set_elm(obj->attr, str->data, str->size);
}
