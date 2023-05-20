#include "op_structs.h"
//struct object_convert op_function_convert = {METHOD_CONVERT &op_function__bool, METHOD_CONVERT &op_function__int, METHOD_CONVERT &op_function__float};
//struct object_type op_function_type = {op_function_OP, NULL, NULL, &op_function_convert, &op_function_math};
struct object_type op_function_type = {OP_FUNCTION_OP};

struct op_function *op_function_new() {
    struct op_function *res = skr_malloc(OP_FUNCTION_SIZE);

    res->closure = NULL;
    res->argument = NULL;
    res->function_body = 0;
    res->argument_size = 0;

    res->func = NULL;

    return res;
}
void op_function_free(struct op_function *res) {
    if (res == NULL) return;
    if(res->closure != NULL) frame_free(res->closure);

    skr_free(res);
}

void op_function_set(struct op_function *res, const struct op_function *a) {
    if (res == NULL) return;
    op_function_clear(res);
    if (a == NULL) return;

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->function_body = a->function_body;
    res->argument_size = a->argument_size;

    res->func = a->func;
}
void op_function_copy(struct op_function *res, const struct op_function *a) {
    if (res == NULL) return;
    op_function_clear(res);
    if (a == NULL) return;

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->function_body = a->function_body;
    res->argument_size = a->argument_size;

    res->func = a->func;
}

void op_function_clear(struct op_function *res) {
    if (res == NULL) return;

    if(res->closure != NULL) frame_free(res->closure);
    res->closure = NULL;
    res->argument = NULL;
    res->function_body = 0;
    res->argument_size = 0;

    res->func = NULL;
}
int op_function_cmp(const struct op_function *obj1, const struct op_function *obj2) {
    if (obj1 == NULL || obj2 == NULL || obj1->function_body != obj2->function_body ||
        obj1->func != obj2->func) return 2;
    return 0;
}

void op_function_define(struct op_function *res, struct node_st *node, struct parser_st *parser) {
    res->argument_size = node->nodes.nodes[0]->nodes.size;
    res->argument = node->variable;
    res->function_body = node->data;

    struct frame_st *frame = res->closure = frame_new();
    struct variable_list_st data = node->closure->data;

    variable_list_set(&frame->attrib, &node->closure->attrib);
    list_resize(&frame->data, data.size);
    for (size_t i = 0; i < data.size; i++) {
        frame->data.data[i] = object_copy_obj(parser->var_stack->data[parser->var_start_pos + data.variables[i]->position]);
    }
}
