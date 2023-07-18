#include <stdio.h>
#include "op_operations.h"


size_t run_codespace(struct parser_st *parser, struct bytecode_st *code, size_t position) {
    char command;
    size_t data_s;
    size_t var_start_pos = parser->var_start_pos;
    struct list_st *const_objects = parser->const_objects;
    struct list_st *temp_stack = parser->temp_stack;
    struct list_st *var_stack = parser->var_stack;

    struct object_st *res = NULL;
    struct object_st *obj1 = NULL;
    struct object_st *obj2 = NULL;

    struct error_st *err = parser->error;


    struct op_class *op_clas = NULL;
    struct op_function *op_func = NULL;
    struct op_object *op_obj = NULL;


    while (position < code->size) {
        command = code->command[position];
        data_s = code->data[position];
        if ((command & 0xF0) == BC_Basics) {
            switch (command) {
                case BC_Init: // Done
                    if (var_stack->data[var_start_pos + data_s] != NULL)
                        object_clear(var_stack->data[var_start_pos + data_s]);
                    else
                        var_stack->data[var_start_pos + data_s] = object_new();
                case BC_Load: // Done
                    if (var_stack->data[var_start_pos + data_s] == NULL) {
                        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
                        return 0;
                    }
                    list_append(temp_stack, var_stack->data[var_start_pos + data_s]);
                    break;
                case BC_LoadConst: // Done
                    if (const_objects->data[data_s] == NULL) {
                        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
                        return 0;
                    }
                    list_append(temp_stack, const_objects->data[data_s]);
                    break;
                case BC_Attrib: {
                    obj1 = object_copy_obj(temp_stack->data[temp_stack->size - 1]);
                    res = object_attrib(err, obj1, const_objects->data[data_s]->data);

                    if (err == NULL && err->present) return 0;
                    else list_append(temp_stack, res);
                    object_free(res);
                    object_free(obj1);
                    break;
                }
                case BC_Attrib_Del: {
                    obj1 = list_pop(temp_stack);
                    res = object_attrib(err, obj1, const_objects->data[data_s]->data);

                    if (err == NULL && err->present) return 0;
                    else list_append(temp_stack, res);
                    object_free(res);
                    object_free(obj1);
                    break;
                }
                case BC_Subscript: {
                    obj1 = list_pop(temp_stack);
                    obj2 = list_pop(temp_stack);
                    res = object_subscript(err, obj1, obj2);

                    if (err == NULL && err->present) return 0;
                    else list_append(temp_stack, res);
                    object_free(res);
                    object_free(obj1);
                    object_free(obj2);
                    break;
                }
                case BC_Call: {
                    obj1 = list_pop(temp_stack);
                    if (obj1->type == OP_CLASS_TYPE) {
                        list_append(temp_stack, obj1);
                        op_clas = obj1->data;
                        parser_store_vars(parser, parser->variables.variable_lists[op_clas->argument]->size, position);
                        if (err == NULL && err->present) return 0;
                        for (size_t i = 0, size = op_clas->closure->attrib.size; i < size; i++) {
                            var_stack->data[parser->var_start_pos +
                                    op_clas->closure->attrib.variables[i]->position] = object_copy_obj(
                                    op_clas->closure->data.data[i]);
                        }

                        code->data[position] ++;
                        bytecode_list_append(&parser->codes_stack, code);
                        bytecode_list_append(&parser->codes_stack, parser->codes.bytecodes[op_clas->class_body]);
                        object_free(obj1);
                        return 0;
                    } else if (obj1->type == OP_FUNCTION_TYPE) {
                        op_func = obj1->data;
                        parser_store_vars(parser, parser->variables.variable_lists[op_func->argument]->size, position + 1);
                        if (err == NULL && err->present) return 0;
                        for (size_t i = 0, size = op_func->closure->attrib.size; i < size; i++) {
                            var_stack->data[parser->var_start_pos +
                                    op_func->closure->attrib.variables[i]->position] = object_copy_obj(
                                    op_func->closure->data.data[i]);
                        }

                        if (op_func->argument_size != data_s) {
                            error_set_msg(err, ErrorType_Convert, "Miss match arguments");
                            return 0;
                        }

                        for (size_t i = 0, size = op_func->argument_size; i < size; i++) {
                            var_stack->data[parser->var_start_pos + i + 1] = list_pop(temp_stack);
                        }
                        bytecode_list_append(&parser->codes_stack, code);
                        bytecode_list_append(&parser->codes_stack, parser->codes.bytecodes[op_func->function_body]);
                        object_free(obj1);
                        return 0;
                    } else if (obj1->type == OP_OBJECT_TYPE) {
                        list_append(temp_stack, obj1);
                        op_obj = obj1->data;
                        parser_store_vars(parser, parser->variables.variable_lists[op_obj->argument]->size, position);
                        if (err == NULL && err->present) return 0;
                        for (size_t i = 0, size = op_obj->closure->attrib.size; i < size; i++) {
                            var_stack->data[parser->var_start_pos +
                                    op_obj->closure->attrib.variables[i]->position] = object_copy_obj(
                                    op_obj->closure->data.data[i]);
                        }

                        code->data[position] ++;
                        bytecode_list_append(&parser->codes_stack, code);
                        bytecode_list_append(&parser->codes_stack, parser->codes.bytecodes[op_obj->class_body]);
                        object_free(obj1);
                        return 0;
                    } else {
                        error_set_msg(err, ErrorType_Convert, "variable is not callable");
                        return 0;
                    }
                }
                case BC_ClassEnd:
                    obj1 = object_new();
                    if (obj1 == NULL) {
                        error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
                        return 0;
                    }
                    obj2 = list_pop(temp_stack);

                    object_set_type(obj1, OP_OBJECT_TYPE);

                    op_object_define(obj1->data, obj2->data);
                    res = op_class_save_value(obj1->data, parser);
                    list_append(temp_stack, obj1);
                    list_append(temp_stack, res);
                    position = parser_restore_vars(parser);
                    if (res == NULL) ++position;

                    object_free(obj1);
                    object_free(obj2);
                    object_free(res);
                    return position;
                case BC_FuncEnd:
                    position = parser_restore_vars(parser);
                    return position;
                case BC_Pop: // Done
                    object_free(list_pop(temp_stack));
                    break;
            }
        } // Not // Done
        else if ((command & 0xF0) == BC_Convert) {
            obj1 = list_pop(temp_stack);
            res = object_new();
            if (res == NULL) {
                error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
                return 0;
            }

            if (command == BC_Convert_Bool) object__bool(res, err, obj1);
            else if (command == BC_Convert_Int) object__int(res, err, obj1);
            else if (command == BC_Convert_Float) object__float(res, err, obj1);
            else if (command == BC_Convert_Str) object__str(res, err, obj1);

            if (err == NULL && err->present) return 0;
            else list_append(temp_stack, res);
            object_free(res);
            object_free(obj1);
        } // Done
        else if ((command & 0xF0) == BC_Operations) {
            obj2 = list_pop(temp_stack);
            res = object_new();
            if (res == NULL) {
                error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
                return 0;
            }

            if (command == BC_Negate) {
                object__neg(res, err, obj2);

                if (err == NULL && err->present) return 0;
                else list_append(temp_stack, res);
            } else if (command == BC_NegateBool) {
                object_set_type(res, INTEGER_TYPE);
                if (integer_is_null(obj2->data)) integer_set_ui(res->data, 1);
                else integer_set_ui(res->data, 0);
                list_append(temp_stack, res);
            } else {
                obj1 = list_pop(temp_stack);

                int bool1 = 1, bool2 = 1;
                switch (command) {
                    case BC_Arithmetic:
                        if (data_s == Special_MOD) object__mod(res, err, obj1, obj2);
                        else if (data_s == Special_AND) object__and(res, err, obj1, obj2);
                        else if (data_s == Special_MUL) object__mul(res, err, obj1, obj2);
                        else if (data_s == Special_ADD) object__add(res, err, obj1, obj2);
                        else if (data_s == Special_SUB) object__sub(res, err, obj1, obj2);
                        else if (data_s == Special_DIV) object__div(res, err, obj1, obj2);
                        else if (data_s == Special_XOR) object__xor(res, err, obj1, obj2);
                        else if (data_s == Special_OR) object__or(res, err, obj1, obj2);
                        else if (data_s == Special_LSHIFT) object__ls(res, err, obj1, obj2);
                        else if (data_s == Special_RSHIFT) object__rs(res, err, obj1, obj2);
                        else {
                            bool1 = object_cmp(obj1, obj2);
                            object_set_type(res, INTEGER_TYPE);

                            if (
                                    (data_s == Special_LESS && bool1 < 0) ||
                                    (data_s == Special_GREATER && bool1 != 2 && bool1 > 0) ||
                                    (data_s == Special_EQ_LESS && bool1 <= 0) ||
                                    (data_s == Special_EQ_GREATER && bool1 != 2 && bool1 >= 0) ||
                                    (data_s == Special_EQ_NOT && bool1 != 0) ||
                                    (data_s == Special_EQ_EQ && bool1 == 0))
                                integer_set_ui(res->data, 1);
                            else integer_set_ui(res->data, 0);

                            list_append(temp_stack, res);
                            break;
                        }

                        if (err == NULL && err->present) return 0;
                        else list_append(temp_stack, res);
                        break;
                    case BC_ArithmeticSet:
                        if (data_s == Special_EQ_MOD) object__mod(res, err, obj1, obj2);
                        else if (data_s == Special_EQ_AND) object__and(res, err, obj1, obj2);
                        else if (data_s == Special_EQ_MUL) object__mul(res, err, obj1, obj2);
                        else if (data_s == Special_EQ_ADD) object__add(res, err, obj1, obj2);
                        else if (data_s == Special_EQ_SUB) object__sub(res, err, obj1, obj2);
                        else if (data_s == Special_EQ_DIV) object__div(res, err, obj1, obj2);
                        else if (data_s == Special_EQ_XOR) object__xor(res, err, obj1, obj2);
                        else if (data_s == Special_EQ_OR) object__or(res, err, obj1, obj2);
                        else if (data_s == Special_EQ_LSHIFT) object__ls(res, err, obj1, obj2);
                        else if (data_s == Special_EQ_RSHIFT) object__rs(res, err, obj1, obj2);
                        else if (data_s == Special_EQ) object_set(res, obj2);

                        if (err == NULL && err->present) return 0;
                        else {
                            object_set(obj1, res);
                            list_append(temp_stack, obj1);
                        }
                        break;
                    case BC_Compare:
                        if (obj1->type == INTEGER_TYPE && integer_is_null(obj1->data)) bool1 = 0;
                        if (obj2->type == INTEGER_TYPE && integer_is_null(obj2->data)) bool2 = 0;

                        object_set_type(res, INTEGER_TYPE);
                        if (data_s == Special_AND_AND) integer_set_ui(res->data, bool1 && bool2);
                        if (data_s == Special_OR_OR) integer_set_ui(res->data, bool1 || bool2);

                        list_append(temp_stack, res);
                        break;
                    case BC_Set:
                        object_set(obj1, obj2);
                        list_append(temp_stack, obj2);
                        break;
                }

                object_free(obj1);
            }
            object_free(res);
            object_free(obj2);
        } // Done
        else if ((command & 0xF0) == BC_Jump) {
            if (command == BC_Jump) {
                position = data_s;
                continue;
            } else {
                struct object_st *obj = temp_stack->data[temp_stack->size - 1];
                int bool = !integer_is_null(obj->data);

                if ((bool && command == BC_IfTrueOrPop) || (!bool && command == BC_IfFalseOrPop) ||
                    (!bool && command == BC_IfFalse_Jump)) {
                    position = data_s;
                    object_free(list_pop(temp_stack));
                    continue;
                } else if (command == BC_IfFalse_Jump) object_free(list_pop(temp_stack));
            }
        }  // Done

        else if ((command & 0xF0) == BC_Make) {
            obj1 = object_new();
            if (obj1 == NULL) {
                error_set_msg(err, ErrorType_RunTime, "Memory Over Flow");
                return 0;
            }
            switch (command) {
                case BC_MakeClass:
                    object_set_type(obj1, OP_CLASS_TYPE);
                    op_class_define(obj1->data, data_s, parser);
                    break;
                case BC_MakeFunc:
                    object_set_type(obj1, OP_FUNCTION_TYPE);
                    op_function_define(obj1->data, data_s, parser);
                    break;
                case BC_MakeList:
                    object_set_type(obj1, LIST_TYPE);
                    obj2 = NULL;
                    for (size_t i = 0; i < data_s; i++) {
                        obj2 = list_pop(temp_stack);
                        list_append(obj1->data, obj2);
                        object_free(obj2);
                    }
                    break;
            }
            list_append(temp_stack, obj1);
            object_free(obj1);
        } // Not // Done
        else if ((command & 0xF0) == BC_Interrupts) {

        }
        ++position;
    }
    return 0;
}

void run_smart_contract(struct parser_st *parser) {
    struct bytecode_list_st *codes_stack = &parser->codes_stack;
    struct bytecode_st *code;

    size_t position = 0;
    while (codes_stack->size > 0 && !parser->error->present) {
        code = bytecode_list_pop(codes_stack);
        position = run_codespace(parser, code, position);
    }
}

void sc_first_run(struct parser_st *parser) {
    parser_store_vars(parser, parser->variables.variable_lists[0]->size, 0);
    if (parser->error == NULL && parser->error->present) return;
    bytecode_list_append(&parser->codes_stack, parser->codes.bytecodes[0]);
    run_smart_contract(parser);
}

struct object_st *sc_run_function(struct parser_st *parser, struct object_st *func, struct list_st *params) {
    if (func->type != OP_FUNCTION_TYPE) return NULL;

    struct op_function *op_func = func->data;
    parser_store_vars(parser, parser->variables.variable_lists[op_func->argument]->size, 0);
    if (parser->error == NULL && parser->error->present) return NULL;
    for (size_t i = 0, size = op_func->closure->attrib.size; i < size; i++) {
        parser->var_stack->data[parser->var_start_pos +
                        op_func->closure->attrib.variables[i]->position] = object_copy_obj(
                op_func->closure->data.data[i]);
    }

    if (op_func->argument_size != params->size) {
        error_set_msg(parser->error, ErrorType_Convert, "Miss match arguments");
        return NULL;
    }

    for (size_t i = 0, size = op_func->argument_size; i < size; i++) {
        parser->var_stack->data[parser->var_start_pos + i + 1] = object_copy_obj(params->data[i]);
    }
    bytecode_list_append(&parser->codes_stack, parser->codes.bytecodes[op_func->function_body]);
    run_smart_contract(parser);
    return list_pop(parser->temp_stack);
}
