#include "op_structs.h"

struct object_sub op_class_sub = {METHOD_SUBSCRIPT &op_class_subscript, METHOD_ATTRIB &op_class_attrib};
//struct object_convert op_class_convert = {METHOD_CONVERT &op_class__bool, METHOD_CONVERT &op_class__int, METHOD_CONVERT &op_class__float, METHOD_CONVERT &op_class__str};
//struct object_type op_class_type = {op_class_OP, NULL, &op_class_sub, &op_class_convert};
struct object_type op_class_type = {OP_CLASS_OP, NULL, &op_class_sub};

struct op_class *op_class_new() {
    struct op_class *res = skr_malloc(OP_CLASS_SIZE);
    res->attr = map_new();

    res->closure = NULL;
    res->argument = NULL;
    res->class_body = 0;

    return res;
}
void op_class_free(struct op_class *res) {
    if (res == NULL) return;
    if(res->closure != NULL) frame_free(res->closure);
    map_free(res->attr);

    skr_free(res);
}

void op_class_set(struct op_class *res, const struct op_class *a) {
    if (res == NULL) return;
    op_class_clear(res);
    if (a == NULL) return;
    map_set(res->attr, a->attr);

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->class_body = a->class_body;
}
void op_class_copy(struct op_class *res, const struct op_class *a) {
    if (res == NULL) return;
    op_class_clear(res);
    if (a == NULL) return;
    map_copy(res->attr, a->attr);

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->class_body = a->class_body;
}

void op_class_clear(struct op_class *res) {
    if (res == NULL) return;
    map_clear(res->attr);

    if(res->closure != NULL) frame_free(res->closure);
    res->closure = NULL;
    res->argument = NULL;
    res->class_body = 0;
}
int op_class_cmp(const struct op_class *obj1, const struct op_class *obj2) {
    if (obj1 == NULL || obj2 == NULL || obj1->class_body != obj2->class_body) return 2;
    return 0;
}

void op_class_define(struct op_class *res, struct node_st *node, struct parser_st *parser) {
    res->argument = node->variable;
    res->class_body = node->data;

    struct frame_st *frame = res->closure = frame_new();
    struct variable_list_st data = node->closure->data;

    variable_list_set(&frame->attrib, &node->closure->attrib);
    list_resize(&frame->data, data.size);
    for (size_t i = 0; i < data.size; i++) {
        frame->data.data[i] = object_copy_obj(parser->var_stack->data[parser->var_start_pos + data.variables[i]->position]);
    }
}
struct object_st *op_class_save_value(struct op_class *res, struct parser_st* parser) {
    struct variable_list_st *attrib = res->argument;
    struct object_st *obj = NULL;
    struct op_function *func = NULL;
    size_t var_start_pos = parser->var_start_pos;


    for (size_t i = 0; i < attrib->size; i++) {
        obj = map_set_elm(res->attr, attrib->variables[i]->name.data, attrib->variables[i]->name.size);
        object_set(obj, parser->var_stack->data[var_start_pos + attrib->variables[i]->position]);
        object_free(obj);
    }

    return map_get_elm(res->attr, "__init__", 8);
}

// Sub method
struct object_st *op_class_subscript(struct object_st *err, struct op_class *obj, const struct object_st *obj2) {
    return map_subscript(err, obj->attr, obj2);
}
struct object_st *op_class_attrib(struct object_st *err, const struct op_class *obj, const struct string_st *str) {
    return map_set_elm(obj->attr, str->data, str->size);
}