#include "op_structs.h"
struct object_sub op_object_sub = {METHOD_SUBSCRIPT &op_object_subscript, METHOD_ATTRIB &op_object_attrib};
//struct object_math_op op_object_math = {METHOD_MATH &op_object__mod, METHOD_MATH &op_object__and, METHOD_MATH &op_object__mul, METHOD_MATH &op_object__add, METHOD_MATH &op_object__sub, METHOD_MATH &op_object__div, METHOD_MATH &op_object__xor, METHOD_MATH &op_object__or, METHOD_MATH &op_object__ls, METHOD_MATH &op_object__rs, METHOD_CONVERT &op_object__neg};
//struct object_convert op_object_convert = {METHOD_CONVERT &op_object__bool, METHOD_CONVERT &op_object__int, METHOD_CONVERT &op_object__float, METHOD_CONVERT &op_object__str};
//struct object_type op_object_type = {OP_OBJECT_OP, NULL, &op_object_sub, &op_object_convert, &op_object_math};
struct object_type op_object_type = {OP_OBJECT_OP, NULL, &op_object_sub};

struct op_object *op_object_new() {
    struct op_object *res = skr_malloc(OP_OBJECT_SIZE);
    res->data = NULL;

    res->class = NULL;
    res->attr = map_new();

    res->closure = NULL;
    res->argument = NULL;
    res->function_body = 0;
    res->argument_size = 0;

    res->func = NULL;

    return res;
}
void op_object_free(struct op_object *res) {
    if (res == NULL) return;
    if(res->data != NULL) object_free(res->data);
    if(res->class != NULL) object_free(res->class);
    if(res->closure != NULL) frame_free(res->closure);
    map_free(res->attr);

    skr_free(res);
}

void op_object_set(struct op_object *res, const struct op_object *a) {
    if (res == NULL) return;
    op_object_clear(res);
    if (a == NULL) return;

    res->data = object_copy_obj(a->data);

    res->class = object_copy_obj(a->class);
    map_set(res->attr, a->attr);

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->function_body = a->function_body;
    res->argument_size = a->argument_size;

    res->func = a->func;
}
void op_object_copy(struct op_object *res, const struct op_object *a) {
    if (res == NULL) return;
    op_object_clear(res);
    if (a == NULL) return;

    res->data = object_new();
    object_copy(res->data, a->data);

    res->class = object_new();
    object_copy(res->class, a->class);
    map_copy(res->attr, a->attr);

    if (a->closure != NULL) {
        res->closure = frame_new();
        frame_set(res->closure, a->closure);
    }
    res->argument = a->argument;
    res->function_body = a->function_body;
    res->argument_size = a->argument_size;

    res->func = a->func;
}

void op_object_clear(struct op_object *res) {
    if (res == NULL) return;
    if(res->data != NULL) object_free(res->data);
    res->data = NULL;

    if(res->class != NULL) object_free(res->class);
    res->class = NULL;
    map_clear(res->attr);

    if(res->closure != NULL) frame_free(res->closure);
    res->closure = NULL;
    res->argument = NULL;
    res->function_body = 0;
    res->argument_size = 0;

    res->func = NULL;
}
int op_object_cmp(const struct op_object *obj1, const struct op_object *obj2) {
    if (obj1 == NULL || obj2 == NULL) return 2;
    return object_cmp(obj1->data, obj2->data);
}

void op_object_set_function(struct op_object *res, struct node_st *node, struct parser_st *parser) {
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
// Sub method
struct object_st *op_object_subscript(struct object_st *err, struct op_object *obj, const struct object_st *obj2) {
    return map_subscript(err, obj->attr, obj2);
}
struct object_st *op_object_attrib(struct object_st *err, const struct op_object *obj, const struct string_st *str) {
    return map_set_elm(obj->attr, str->data, str->size);
}


void f(){
//// Convert Methods
//    void op_object__bool(struct object_st *res, struct object_st *err, const struct op_object *obj1) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__bool__", 8);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __bool__ not realized", 31);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 1) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__int(struct object_st *res, struct object_st *err, const struct op_object *obj1) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__int__", 7);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __int__ not realized", 30);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 1) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__float(struct object_st *res, struct object_st *err, const struct op_object *obj1) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__float__", 9);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __float__ not realized", 32);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 1) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__str(struct object_st *res, struct object_st *err, const struct op_object *obj1) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__str__", 7);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __str__ not realized", 30);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 1) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//
//// Math Methods
//    void op_object__mod(struct object_st *res, struct object_st *err, const struct op_object *obj1, const struct object_st *obj2) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__mod__", 7);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __mod__ not realized", 30);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 2) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    func->argument->attrib.attribs[1]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//                    object_set(func->argument->attrib.attribs[1]->data, obj2);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__and(struct object_st *res, struct object_st *err, const struct op_object *obj1, const struct object_st *obj2) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__and__", 7);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __and__ not realized", 30);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 2) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    func->argument->attrib.attribs[1]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//                    object_set(func->argument->attrib.attribs[1]->data, obj2);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__mul(struct object_st *res, struct object_st *err, const struct op_object *obj1, const struct object_st *obj2) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__mul__", 7);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __mul__ not realized", 30);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 2) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    func->argument->attrib.attribs[1]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//                    object_set(func->argument->attrib.attribs[1]->data, obj2);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__add(struct object_st *res, struct object_st *err, const struct op_object *obj1, const struct object_st *obj2) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__add__", 7);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __add__ not realized", 30);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 2) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    func->argument->attrib.attribs[1]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//                    object_set(func->argument->attrib.attribs[1]->data, obj2);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__sub(struct object_st *res, struct object_st *err, const struct op_object *obj1, const struct object_st *obj2) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__sub__", 7);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __sub__ not realized", 30);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 2) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    func->argument->attrib.attribs[1]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//                    object_set(func->argument->attrib.attribs[1]->data, obj2);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__div(struct object_st *res, struct object_st *err, const struct op_object *obj1, const struct object_st *obj2) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__div__", 7);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __div__ not realized", 30);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 2) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    func->argument->attrib.attribs[1]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//                    object_set(func->argument->attrib.attribs[1]->data, obj2);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__xor(struct object_st *res, struct object_st *err, const struct op_object *obj1, const struct object_st *obj2) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__xor__", 7);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __xor__ not realized", 30);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 2) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    func->argument->attrib.attribs[1]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//                    object_set(func->argument->attrib.attribs[1]->data, obj2);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__or(struct object_st *res, struct object_st *err, const struct op_object *obj1, const struct object_st *obj2) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__or__", 6);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __or__ not realized", 29);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 2) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    func->argument->attrib.attribs[1]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//                    object_set(func->argument->attrib.attribs[1]->data, obj2);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__ls(struct object_st *res, struct object_st *err, const struct op_object *obj1, const struct object_st *obj2) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__ls__", 6);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __ls__ not realized", 29);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 2) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    func->argument->attrib.attribs[1]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//                    object_set(func->argument->attrib.attribs[1]->data, obj2);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__rs(struct object_st *res, struct object_st *err, const struct op_object *obj1, const struct object_st *obj2) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__rs__", 6);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __rs__ not realized", 29);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 2) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    func->argument->attrib.attribs[1]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//                    object_set(func->argument->attrib.attribs[1]->data, obj2);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
//    void op_object__neg(struct object_st *res, struct object_st *err, const struct op_object *obj1) {
//        struct string_st *operation = string_new();
//        string_set_str(operation, "__neg__", 7);
//        struct object_st *obj = map_get_elm(obj1->attr, operation->data, operation->size);
//        string_free(operation);
//        if (obj == NULL) {
//            object_set_type(err, STRING_TYPE);
//            string_set_str(err->data, "operation __neg__ not realized", 30);
//        }
//        else if (obj->type == OP_OBJECT_TYPE) {
//            struct op_object *func = obj->data;
//            if (func->func != NULL) {
//                // TODO call local function
//            } else if(func->function_body != NULL) {
//                size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                if (need == 1) {
//                    struct parser_st parser;
//                    parser_data_inti(&parser);
//
//                    frame_save_data(frame_list_add_new(&parser.frame_stack), &func->argument->attrib);
//
//                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                        if (func->closure->attrib.attribs[i]->data != NULL)
//                            object_free(func->closure->attrib.attribs[i]->data);
//                        func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                    }
//
//                    func->argument->attrib.attribs[0]->data = object_new();
//                    object_set_type(func->argument->attrib.attribs[0]->data, OP_OBJECT_TYPE);
//                    op_object_set(func->argument->attrib.attribs[0]->data->data, obj1);
//
//                    block_list_append(&parser.blocks_stack, func->function_body, 0);
//                    struct block_expr_st *block = NULL;
//                    while (parser.blocks_stack.size) {
//                        block = block_list_last(&parser.blocks_stack);
//                        size_t position = block_list_last_pos(&parser.blocks_stack);
//                        block_list_resize(&parser.blocks_stack, parser.blocks_stack.size - 1);
//
//                        run_block(&parser, block, position);
//                    }
//                    if (parser.temp_memory->size != 0) {
//                        if (parser.interrupt_type == BC_Throw) {
//                            struct object_st *temp = list_pop(parser.temp_memory);
//                            object_set(err, temp);
//                            object_free(temp);
//                        }
//
//                        struct object_st *temp = list_pop(parser.temp_memory);
//                        object_set(res, temp);
//                        object_free(temp);
//                    }
//                    parser_data_free(&parser);
//                }
//                else {
//                    object_set_type(err, STRING_TYPE);
//                    string_set_str(err->data, "incorrect number of arguments", 30);
//                }
//            }
//        }
//        else {
//            object_set(res, obj);
//        }
//        object_free(obj);
//    }
}