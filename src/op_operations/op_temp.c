#include <stdio.h>
#include "op_operations.h"


void lol() {
//    if (interrupt_type != 0) {
////        if (interrupt_type == Interrupt_Break && block->type == BlockType_Conf &&
////            block->sub_type == ScopeType_StmtLoop) {
////            interrupt_type = 0;
////            return;
////        } else if (block->type != BlockType_OP || block->sub_type != Block_FrameClose) return;
//    }
//    struct block_list_st *blocks_stack = &blocks_stack;
//    struct block_expr_st *block_temp = NULL;
//    if (block->type == BlockType_None && block->node != NULL) {
//        block->code = bytecode_new();
//        run_make_bytecode(parser, block->node, block->code);
//        print_bytecode(block->code, 0);
//        block->type = BlockType_ByteCode;
//    }
//    if (block->type == BlockType_ByteCode) {
//        struct bytecode_st *code = block->code;
//        size_t pos = position;
//        if(interrupt_type == Interrupt_Break_Out) {
//            pos ++;
//            interrupt_type = 0;
//        }
//        while (pos < code->size) {
//            char command = code->command[pos];
//            void *data = code->data[pos];
//            if (interrupt_type == 0 || (command & 0xF0) == BC_Frame) {
//                if ((command & 0xF0) == 0) {
//                    switch (command) {
//                        case BC_Init:
//                            if (((struct attrib_st *) data)->data != NULL)
//                                object_free(((struct attrib_st *) data)->data);
//                            ((struct attrib_st *) data)->data = object_new();
//                        case BC_Load:
////                            print_obj(((struct attrib_st *) data)->data, 0);
//                            list_append(temp_stack, ((struct attrib_st *) data)->data);
//                            break;
//                        case BC_LoadConst:
//                            list_append(temp_stack, data);
//                            break;
//                        case BC_Attrib: {
//                            struct object_st *obj = list_pop(temp_stack);
//                            struct object_st *err = object_new();
//                            struct object_st *res = object_attrib(err, obj, ((struct object_st *)data)->data);
//                            if (err->type != NONE_TYPE) {
//                                interrupt_type = Interrupt_Throw;
//                                interrupt_scopes = 0;
//                                list_append(temp_stack, err);
//                            } else {
//                                list_append(temp_stack, res);
//                            }
//                            object_free(res);
//                            object_free(err);
//                            object_free(obj);
//                            break;
//                        }
//                        case BC_Subscript: {
//                            struct object_st *obj1 = list_pop(temp_stack);
//                            struct object_st *obj2 = list_pop(temp_stack);
//                            struct object_st *err = object_new();
//                            struct object_st *res = object_subscript(err, obj1, obj2);
//                            if (err->type != NONE_TYPE) {
//                                interrupt_type = Interrupt_Throw;
//                                interrupt_scopes = 0;
//                                list_append(temp_stack, err);
//                            } else {
//                                list_append(temp_stack, res);
//                            }
//                            object_free(res);
//                            object_free(err);
//                            object_free(obj1);
//                            object_free(obj2);
//                            break;
//                        }
//                        case BC_Call: {
//
//                            struct object_st *temp = NULL;
//                            struct object_st *obj = list_pop(temp_stack);
//
//                            if (obj->type != OP_OBJECT_TYPE) {
//                                // TODO error
//                                exit(1);
//                            }
//                            struct op_object *func = obj->data;
//                            {
//
//                                frame_save_data(frame_list_add_new(&frame_stack), &func->argument->attrib);
//
//                                for (size_t i = 0; i < func->closure->attrib.size; i++) {
//                                    if (func->closure->attrib.attribs[i]->data != NULL)
//                                        object_free(func->closure->attrib.attribs[i]->data);
//                                    func->closure->attrib.attribs[i]->data = object_copy_obj(func->closure->data.data[i]);
//                                }
//                            }
//
//                            size_t need = func->argument->attrib.size - func->closure->attrib.size;
//                            if (NULL + need != data) {
//                                // TODo error
//                                exit(1);
//                            }
//
//                            for (int i = 0; i < need; i++) {
//                                func->argument->attrib.attribs[i]->data = object_new();
//                                temp = list_pop(temp_stack);
//                                object_set(func->argument->attrib.attribs[i]->data, temp);
//                                object_free(temp);
//                            }
//
//                            block_list_append(blocks_stack, block, pos + 1);
//                            block_list_append(blocks_stack, func->function_body, 0);
//                            return;
//                        }
//                        case BC_Pop:
//                            object_free(list_pop(temp_stack));
//                            break;
//                    }
//                } // Not // Done
//                else if ((command & 0xF0) == BC_Convert) {
//                    struct object_st *temp = NULL;
//                    struct object_st *obj = list_pop(temp_stack);
//
//                    struct object_st *res = object_new();
//                    struct object_st *err = object_new();
//
//                    if (command == BC_Convert_Bool) object__bool(res, err, obj);
//                    else if (command == BC_Convert_Int) object__int(res, err, obj);
//                    else if (command == BC_Convert_Float) object__float(res, err, obj);
//                    else if (command == BC_Convert_Str) object__str(res, err, obj);
//
//                    if (err->type != NONE_TYPE) {
//                        interrupt_type = Interrupt_Throw;
//                        interrupt_scopes = 0;
//                        list_append(temp_stack, err);
//                    } else {
//                        list_append(temp_stack, res);
//                    }
//                    object_free(res);
//                    object_free(err);
//
//                    object_free(obj);
//                    object_free(temp);
//                } // Done
//                else if ((command & 0xF0) == BC_Operations) {
//                    struct object_st *obj2 = list_pop(temp_stack);
//                    struct object_st *res = object_new();
//                    struct object_st *err = object_new();
//                    if (command == BC_Negate) {
//                        object__neg(res, err, obj2);
//                        if (err->type != NONE_TYPE) {
//                            interrupt_type = Interrupt_Throw;
//                            interrupt_scopes = 0;
//                            list_append(temp_stack, err);
//                        } else {
//                            list_append(temp_stack, res);
//                        }
//                    } else if (command == BC_NegateBool) {
//                        object_set_type(res, INTEGER_TYPE);
//                        if (integer_is_null(obj2->data)) integer_set_ui(res->data, 1);
//                        else integer_set_ui(res->data, 0);
//                        list_append(temp_stack, res);
//                    } else {
//                        struct object_st *obj1 = list_pop(temp_stack);
//
//                        int bool1 = 1, bool2 = 1;
//                        switch (command) {
//                            case BC_Arithmetic:
//                                if ((int) data == Special_MOD) object__mod(res, err, obj1, obj2);
//                                else if ((int) data == Special_AND) object__and(res, err, obj1, obj2);
//                                else if ((int) data == Special_MUL) object__mul(res, err, obj1, obj2);
//                                else if ((int) data == Special_ADD) object__add(res, err, obj1, obj2);
//                                else if ((int) data == Special_SUB) object__sub(res, err, obj1, obj2);
//                                else if ((int) data == Special_DIV) object__div(res, err, obj1, obj2);
//                                else if ((int) data == Special_XOR) object__xor(res, err, obj1, obj2);
//                                else if ((int) data == Special_OR) object__or(res, err, obj1, obj2);
//                                else if ((int) data == Special_LSHIFT) object__ls(res, err, obj1, obj2);
//                                else if ((int) data == Special_RSHIFT) object__rs(res, err, obj1, obj2);
//                                else {
//                                    bool1 = object_cmp(obj1, obj2);
//                                    object_set_type(res, INTEGER_TYPE);
//
//                                    if (
//                                            ((int) data == Special_LESS && bool1 < 0) ||
//                                            ((int) data == Special_GREATER && bool1 != 2 && bool1 > 0) ||
//                                            ((int) data == Special_EQ_LESS && bool1 <= 0) ||
//                                            ((int) data == Special_EQ_GREATER && bool1 != 2 && bool1 >= 0) ||
//                                            ((int) data == Special_EQ_NOT && bool1 != 0) ||
//                                            ((int) data == Special_EQ_EQ && bool1 == 0))
//                                        integer_set_ui(res->data, 1);
//                                    else integer_set_ui(res->data, 0);
//
//                                    list_append(temp_stack, res);
//                                    break;
//                                }
//                                if (err->type != NONE_TYPE) {
//                                    interrupt_type = Interrupt_Throw;
//                                    interrupt_scopes = 0;
//                                    list_append(temp_stack, err);
//                                } else list_append(temp_stack, res);
//                                break;
//                            case BC_ArithmeticSet:
//                                if ((int) data == Special_EQ_MOD) object__mod(res, err, obj1, obj2);
//                                else if ((int) data == Special_EQ_AND) object__and(res, err, obj1, obj2);
//                                else if ((int) data == Special_EQ_MUL) object__mul(res, err, obj1, obj2);
//                                else if ((int) data == Special_EQ_ADD) object__add(res, err, obj1, obj2);
//                                else if ((int) data == Special_EQ_SUB) object__sub(res, err, obj1, obj2);
//                                else if ((int) data == Special_EQ_DIV) object__div(res, err, obj1, obj2);
//                                else if ((int) data == Special_EQ_XOR) object__xor(res, err, obj1, obj2);
//                                else if ((int) data == Special_EQ_OR) object__or(res, err, obj1, obj2);
//                                else if ((int) data == Special_EQ_LSHIFT) object__ls(res, err, obj1, obj2);
//                                else if ((int) data == Special_EQ_RSHIFT) object__rs(res, err, obj1, obj2);
//                                else if ((int) data == Special_EQ) object_set(res, obj2);
//
//                                if (err->type != NONE_TYPE) {
//                                    interrupt_type = Interrupt_Throw;
//                                    interrupt_scopes = 0;
//                                    list_append(temp_stack, err);
//                                } else {
//                                    object_set(obj1, res);
//                                    list_append(temp_stack, obj1);
//                                }
//                                break;
//                            case BC_Compare:
//                                if (obj1->type == INTEGER_TYPE && integer_is_null(obj1->data)) bool1 = 0;
//                                if (obj2->type == INTEGER_TYPE && integer_is_null(obj2->data)) bool2 = 0;
//
//                                object_set_type(res, INTEGER_TYPE);
//                                if ((int) data == Special_AND_AND) integer_set_ui(res->data, bool1 && bool2);
//                                if ((int) data == Special_OR_OR) integer_set_ui(res->data, bool1 || bool2);
//
//                                list_append(temp_stack, res);
//                                break;
//                            case BC_Set:
//                                object_set(obj1, obj2);
//                                list_append(temp_stack, obj2);
//                                break;
//                        }
//
//                        object_free(obj1);
//                    }
//                    object_free(res);
//                    object_free(err);
//                    object_free(obj2);
//                } // Done
//                else if ((command & 0xF0) == BC_Jump) {
//                    if (command == BC_Jump) {
//                        pos = (size_t) data;
//                        continue;
//                    }
//                    else if (command == BC_JumpBlock) {
//                        block_list_append(blocks_stack, block, pos + 1);
//                        block_list_append(blocks_stack, data, 0);
//                        return;
//                    }
//                    else if (command == BC_SaveStack) {
//                        // TODO save stack size
//                    }
//                    else if (command == BC_LoadStack) {
//                        struct object_st *obj = list_pop(temp_stack);
//                        size_t size; // TODO load stack size
//                        list_resize(temp_stack, size);
//                        list_append(temp_stack, obj);
//                        object_free(obj);
//                    }
//                    else {
////                        struct object_st *obj = list_pop(temp_stack);
////                        int bool = !integer_is_null(obj->data);
////                        object_free(obj);
////
////                        if ((command == BC_IfTrue_Jump && bool) || (command == BC_IfFalse_Jump && !bool)) {
////                            pos = (size_t) data;
////                            continue;
////                        }
//                    }
//                } // Done
//                else if ((command & 0xF0) == BC_Make) {
//                    struct object_st *obj = object_new();
//                    switch (command) {
//                        case BC_MakeFunc:
//                            object_set_type(obj, OP_OBJECT_TYPE);
//                            block_list_add_new(&blocks);
//                            block_temp = block_list_last(&blocks);
//                            block_temp->sub_type = ScopeType_Func;
//                            block_temp->node = data;
//                            block_temp->node = block_temp->node->nodes.nodes[1];
//                            op_object_set_function(obj->data, data, block_temp);
//                            break;
//                        case BC_MakeClass:
//                            break;
//                        case BC_MakeList:
//                            object_set_type(obj, LIST_TYPE);
//                            struct object_st *temp = NULL;
//                            for (size_t i = 0; i < (size_t) data; i++) {
//                                temp = list_pop(temp_stack);
//                                list_append(obj->data, temp);
//                                object_free(temp);
//                            }
//                            break;
//                    }
//                    list_append(temp_stack, obj);
//                    object_free(obj);
//                } // Not // Done
//                else if ((command & 0xF0) == BC_Frame) {
//                    if (command == BC_FrameInit && interrupt_type == 0) {
//                        if (interrupt_type != 0) interrupt_scopes++;
//                        else frame_save_data(frame_list_add_new(&frame_stack), data);
//                    } else
//                    if (command == BC_FrameClose) {
//                        if (interrupt_type != 0 && interrupt_scopes != 0) interrupt_scopes --;
//                        else frame_load_data(frame_list_pop(&frame_stack));
//                    } else
//                    if (command == BC_FrameCloseFunc && interrupt_type == 0) {
//                        frame_load_data(frame_list_pop(&frame_stack));
//                    }
//                }// Done
//                else if ((command & 0xF0) == BC_Interrupts) {
//                    if (command == BC_Break) interrupt_type = Interrupt_Break;
//                    else if (command == BC_Throw) interrupt_type = Interrupt_Throw;
//                    else if (command == BC_Return) interrupt_type = Interrupt_Return;
//                    else if (command == BC_Continue) interrupt_type = Interrupt_Continue;
//                    interrupt_scopes = 0;
//                } // Not // Done
//            }
//            ++pos;
//        }
//        if(block->sub_type == ScopeType_Func && interrupt_type == Interrupt_Return) interrupt_type = 0;
//        if(block->sub_type == ScopeType_Loop && interrupt_type == Interrupt_Continue) interrupt_type = 0;
//        if(block->sub_type == ScopeType_Loop && interrupt_type == Interrupt_Break) interrupt_type = Interrupt_Break_Out;
//        if(block->sub_type == ScopeType_Try && interrupt_type == Interrupt_Throw) interrupt_type = 0;
//    }
}

size_t run_codespace(struct parser_st *parser, struct bytecode_st *code, size_t position) {
    char command;
    void *data;
    size_t data_s;
    size_t var_start_pos = parser->var_start_pos;
    struct list_st *const_objects = parser->const_objects;
    struct list_st *temp_stack = parser->temp_stack;
    struct list_st *var_stack = parser->var_stack;

    struct object_st **temp;
    struct object_st *res = NULL;
    struct object_st *err = NULL;
    struct object_st *obj1 = NULL;
    struct object_st *obj2 = NULL;
    struct op_object *func = NULL;


    while (position < code->size) {
        command = code->command[position];
        data = code->data[position];
        data_s = (size_t) data;
        if ((command & 0xF0) == BC_Basics) {
            switch (command) {
                case BC_Init: // Done
                    if (var_stack->data[var_start_pos + data_s] != NULL)
                        object_clear(var_stack->data[var_start_pos + data_s]);
                    else
                        var_stack->data[var_start_pos + data_s] = object_new();
                case BC_Load: // Done
                    list_append(temp_stack, var_stack->data[var_start_pos + data_s]);
                    break;
                case BC_LoadConst: // Done
                    list_append(temp_stack, const_objects->data[data_s]);
                    break;
                case BC_Attrib: {
                    obj1 = list_pop(temp_stack);
                    err = object_new();
                    res = object_attrib(err, obj1, const_objects->data[data_s]->data);
                    if (err->type != NONE_TYPE) {
//                        interrupt_type = Interrupt_Throw;
//                        interrupt_scopes = 0;
                        list_append(temp_stack, err);
                    } else list_append(temp_stack, res);
                    object_free(res);
                    object_free(err);
                    object_free(obj1);
                    break;
                }
                case BC_Subscript: {
                    obj1 = list_pop(temp_stack);
                    obj2 = list_pop(temp_stack);
                    err = object_new();
                    res = object_subscript(err, obj1, obj2);
                    if (err->type != NONE_TYPE) {
//                        interrupt_type = Interrupt_Throw;
//                        interrupt_scopes = 0;
                        list_append(temp_stack, err);
                    } else list_append(temp_stack, res);
                    object_free(res);
                    object_free(err);
                    object_free(obj1);
                    object_free(obj2);
                    break;
                }
                case BC_Call: {

//                    print_list(var_stack, 0);
//                    print_list(temp_stack, 0);
                    obj1 = list_pop(temp_stack);
                    func = obj1->data;
                    parser_store_vars(parser, func->argument->size, position + 1);
                    for (size_t i = 0; i < func->closure->attrib.size; i++) {
                        var_stack->data[parser->var_start_pos +
                                        func->closure->attrib.variables[i]->position] = object_copy_obj(
                                func->closure->data.data[i]);
                    }

                    if (func->argument_size != data_s) {
                        // TODo error
                        exit(2);
                    }

                    for (int i = 0; i < func->argument_size; i++) {
                        var_stack->data[parser->var_start_pos + i + 1] = list_pop(temp_stack);
                    }
                    bytecode_list_append(&parser->codes_stack, code);
                    bytecode_list_append(&parser->codes_stack, parser->codes.bytecodes[func->function_body]);
                    object_free(obj1);
                    return 0;
                }
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
            err = object_new();

            if (command == BC_Convert_Bool) object__bool(res, err, obj1);
            else if (command == BC_Convert_Int) object__int(res, err, obj1);
            else if (command == BC_Convert_Float) object__float(res, err, obj1);
            else if (command == BC_Convert_Str) object__str(res, err, obj1);

            if (err->type != NONE_TYPE) {
//                interrupt_type = Interrupt_Throw;
//                interrupt_scopes = 0;
                list_append(temp_stack, err);
            } else list_append(temp_stack, res);
            object_free(res);
            object_free(err);
            object_free(obj1);
        } // Done
        else if ((command & 0xF0) == BC_Operations) {
            obj2 = list_pop(temp_stack);
            res = object_new();
            err = object_new();

            if (command == BC_Negate) {
                object__neg(res, err, obj2);
                if (err->type != NONE_TYPE) {
                    list_append(temp_stack, err);
                } else list_append(temp_stack, res);
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
                        if ((int) data == Special_MOD) object__mod(res, err, obj1, obj2);
                        else if ((int) data == Special_AND) object__and(res, err, obj1, obj2);
                        else if ((int) data == Special_MUL) object__mul(res, err, obj1, obj2);
                        else if ((int) data == Special_ADD) object__add(res, err, obj1, obj2);
                        else if ((int) data == Special_SUB) object__sub(res, err, obj1, obj2);
                        else if ((int) data == Special_DIV) object__div(res, err, obj1, obj2);
                        else if ((int) data == Special_XOR) object__xor(res, err, obj1, obj2);
                        else if ((int) data == Special_OR) object__or(res, err, obj1, obj2);
                        else if ((int) data == Special_LSHIFT) object__ls(res, err, obj1, obj2);
                        else if ((int) data == Special_RSHIFT) object__rs(res, err, obj1, obj2);
                        else {
                            bool1 = object_cmp(obj1, obj2);
                            object_set_type(res, INTEGER_TYPE);

                            if (
                                    ((int) data == Special_LESS && bool1 < 0) ||
                                    ((int) data == Special_GREATER && bool1 != 2 && bool1 > 0) ||
                                    ((int) data == Special_EQ_LESS && bool1 <= 0) ||
                                    ((int) data == Special_EQ_GREATER && bool1 != 2 && bool1 >= 0) ||
                                    ((int) data == Special_EQ_NOT && bool1 != 0) ||
                                    ((int) data == Special_EQ_EQ && bool1 == 0))
                                integer_set_ui(res->data, 1);
                            else integer_set_ui(res->data, 0);

                            list_append(temp_stack, res);
                            break;
                        }
                        if (err->type != NONE_TYPE) {
//                            interrupt_type = Interrupt_Throw;
//                            interrupt_scopes = 0;
                            list_append(temp_stack, err);
                        } else list_append(temp_stack, res);
                        break;
                    case BC_ArithmeticSet:
                        if ((int) data == Special_EQ_MOD) object__mod(res, err, obj1, obj2);
                        else if ((int) data == Special_EQ_AND) object__and(res, err, obj1, obj2);
                        else if ((int) data == Special_EQ_MUL) object__mul(res, err, obj1, obj2);
                        else if ((int) data == Special_EQ_ADD) object__add(res, err, obj1, obj2);
                        else if ((int) data == Special_EQ_SUB) object__sub(res, err, obj1, obj2);
                        else if ((int) data == Special_EQ_DIV) object__div(res, err, obj1, obj2);
                        else if ((int) data == Special_EQ_XOR) object__xor(res, err, obj1, obj2);
                        else if ((int) data == Special_EQ_OR) object__or(res, err, obj1, obj2);
                        else if ((int) data == Special_EQ_LSHIFT) object__ls(res, err, obj1, obj2);
                        else if ((int) data == Special_EQ_RSHIFT) object__rs(res, err, obj1, obj2);
                        else if ((int) data == Special_EQ) object_set(res, obj2);

                        if (err->type != NONE_TYPE) {
//                            interrupt_type = Interrupt_Throw;
//                            interrupt_scopes = 0;
                            list_append(temp_stack, err);
                        } else {
                            object_set(obj1, res);
                            list_append(temp_stack, obj1);
                        }
                        break;
                    case BC_Compare:
                        if (obj1->type == INTEGER_TYPE && integer_is_null(obj1->data)) bool1 = 0;
                        if (obj2->type == INTEGER_TYPE && integer_is_null(obj2->data)) bool2 = 0;

                        object_set_type(res, INTEGER_TYPE);
                        if ((int) data == Special_AND_AND) integer_set_ui(res->data, bool1 && bool2);
                        if ((int) data == Special_OR_OR) integer_set_ui(res->data, bool1 || bool2);

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
            object_free(err);
            object_free(obj2);
        } // Done
        else if ((command & 0xF0) == BC_Jump) {
            if (command == BC_Jump) {
                position = (size_t) data;
                continue;
            } else {
                struct object_st *obj = temp_stack->data[temp_stack->size - 1];
                int bool = !integer_is_null(obj->data);

                if ((bool && command == BC_IfTrueOrPop) || (!bool && command == BC_IfFalseOrPop) ||
                    (!bool && command == BC_IfFalse_Jump)) {
                    position = (size_t) data;
                    object_free(list_pop(temp_stack));
                    continue;
                } else if (command == BC_IfFalse_Jump) object_free(list_pop(temp_stack));
            }
        }  // Done

        else if ((command & 0xF0) == BC_Make) {
            obj1 = object_new();
            switch (command) {
                case BC_MakeFunc:
                    object_set_type(obj1, OP_OBJECT_TYPE);
                    op_object_set_function(obj1->data, data, parser);
                    break;
                case BC_MakeClass:
                    break;
                case BC_MakeList:
                    object_set_type(obj1, LIST_TYPE);
                    obj2 = NULL;
                    for (size_t i = 0; i < (size_t) data; i++) {
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

void run_smart_contract(struct parser_st *parser, size_t codespace) {
    struct bytecode_list_st *codes_stack = &parser->codes_stack;
    struct bytecode_st *code;

    bytecode_list_append(codes_stack, parser->codes.bytecodes[codespace]);
    size_t position = 0;
    printf("------------------------------Code Started--------------------------------------\n");
    while (codes_stack->size > 0) {
        code = bytecode_list_pop(codes_stack);
        position = run_codespace(parser, code, position);
    }
    printf("-------------------------------Code Ended---------------------------------------\n");
    print_list(parser->temp_stack, 0);
    print_list(parser->var_stack, 0);
}