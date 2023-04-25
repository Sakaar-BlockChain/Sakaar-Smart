//#include <stdio.h>
//#include "op_operations.h"
//
//void run_make_bytecode(struct parser_st *parser, struct node_st *node, struct bytecode_st *code) {
//    if (node->type == MainType_Expr) {
//        switch (node->sub_type) {
//            case PrimType_List:
//                for (size_t i = node->nodes.size; i > 0; i--) {
//                    run_make_bytecode(parser, node->nodes.nodes[i - 1], code);
//                }
//                bytecode_append(code, BC_MakeList, NULL + node->nodes.size);
//                break;
//            case PrimType_Ident_get:
//                bytecode_append(code, BC_Load, node->attrib);
//                break;
//            case PrimType_Ident_new:
//                bytecode_append(code, BC_Init, node->attrib);
//                break;
//            case PrimType_Literal:
//                bytecode_append(code, BC_LoadConst, node->data);
//                break;
//            case PrimType_Attrib:
//                run_make_bytecode(parser, node->nodes.nodes[0], code);
//                bytecode_append(code, BC_Attrib, node->data);
//                break;
//            case PrimType_Subscript:
//                run_make_bytecode(parser, node->nodes.nodes[1], code);
//                run_make_bytecode(parser, node->nodes.nodes[0], code);
//                bytecode_append(code, BC_Subscript, NULL);
//                break;
//            case PrimType_Call: {
//                for (size_t i = node->nodes.nodes[1]->nodes.size; i > 0; i--) {
//                    run_make_bytecode(parser, node->nodes.nodes[1]->nodes.nodes[i - 1], code);
//                }
//                run_make_bytecode(parser, node->nodes.nodes[0], code);
//                bytecode_append(code, BC_Call, NULL + node->nodes.nodes[1]->nodes.size);
//                bytecode_append(code, BC_FrameCloseFunc, NULL);
//                break;
//            }
//        }
//    } // Done
//    else if (node->type == MainType_Oper) {
//        size_t count = 2;
//        int is_bool = 0;
//        if (node->sub_type == ExprType_U || node->sub_type == ExprType_NotTest) count = 1;
//        if (node->sub_type == ExprType_AndTest || node->sub_type == ExprType_NotTest ||
//            node->sub_type == ExprType_OrTest)
//            is_bool = 1;
//
//
//        for (size_t i = 0; i < node->nodes.size; i++) {
//            run_make_bytecode(parser, node->nodes.nodes[i], code);
//            if (is_bool) bytecode_append(code, BC_Convert_Bool, NULL);
//        }
//
//        for (size_t i = node->tokens.size; i > 0; i--) {
//            if (count == 2) {
//                if (is_bool) bytecode_append(code, BC_Compare, NULL + node->tokens.tokens[i - 1]->sub_type);
//                else bytecode_append(code, BC_Arithmetic, NULL + node->tokens.tokens[i - 1]->sub_type);
//            } else {
//                if (is_bool) bytecode_append(code, BC_NegateBool, NULL);
//                else bytecode_append(code, BC_Negate, NULL);
//            }
//        }
//    } // Done
//    else if (node->type == MainType_Stmt) {
//        switch (node->sub_type) {
//            case StmtType_Assign:
//                for (size_t i = 0; i < node->nodes.size; i++)
//                    run_make_bytecode(parser, node->nodes.nodes[i], code);
//                for (size_t i = node->tokens.size; i > 0; i--)
//                    bytecode_append(code, BC_ArithmeticSet, NULL + node->tokens.tokens[i - 1]->sub_type);
//
//                bytecode_append(code, BC_Pop, NULL);
//                break;
//            case StmtType_Return:
//                if (node->nodes.size != 0) run_make_bytecode(parser, node->nodes.nodes[0], code);
//                bytecode_append(code, BC_Return, NULL);
//                break;
//            case StmtType_Break:
//                bytecode_append(code, BC_Break, NULL);
//                break;
//            case StmtType_Continue:
//                bytecode_append(code, BC_Continue, NULL);
//                break;
//            case StmtType_Oper:
//                run_make_bytecode(parser, node->nodes.nodes[0], code);
//                bytecode_append(code, BC_Pop, NULL);
//                break;
//            case StmtType_Extends:
//                printf("StmtType_Extends\n");
//                break;
//            case StmtType_Func_Body:
//                bytecode_append(code, BC_MakeFunc, node);
//                break;
//            case StmtType_If: {
////                void ***array = skr_malloc(sizeof(void **) * (node->nodes.size / 2));
////                void **point = NULL;
////                for (size_t i = 1; i < node->nodes.size; i += 2) {
////                    block_list_add_new(&parser->blocks);
////                    struct block_expr_st *block_temp = block_list_last(&parser->blocks);
////                    block_temp->sub_type = ScopeType_None;
////                    block_temp->node = node->nodes.nodes[i];
////
////                    run_make_bytecode(parser, node->nodes.nodes[i - 1], code);
////                    bytecode_append(code, BC_Convert_Bool, NULL);
////
////                    bytecode_append(code, BC_IfFalse_Jump, NULL);
////                    point = &code->data[code->size - 1];
////                    bytecode_append(code, BC_JumpBlock, block_temp);
////                    bytecode_append(code, BC_Jump, NULL);
////                    *point = NULL + code->size;
////                    array[i / 2] = &code->data[code->size - 1];
////                }
////                if (node->nodes.size % 2 == 1) {
////                    block_list_add_new(&parser->blocks);
////                    struct block_expr_st *block_temp = block_list_last(&parser->blocks);
////                    block_temp->sub_type = ScopeType_None;
////                    block_temp->node = node->nodes.nodes[node->nodes.size - 1];
////
////                    bytecode_append(code, BC_JumpBlock, block_temp);
////                }
////                for (size_t i = 1; i < node->nodes.size; i += 2) {
////                    *array[i / 2] = NULL + code->size;
////                }
////                skr_free(array);
//                break;
//            }
//            case StmtType_While: {
////                block_list_add_new(&parser->blocks);
////                struct block_expr_st *block_temp = block_list_last(&parser->blocks);
////                block_temp->sub_type = ScopeType_Loop;
////                block_temp->node = node->nodes.nodes[1];
////
////                size_t pos = code->size;
////                run_make_bytecode(parser, node->nodes.nodes[0], code);
////                bytecode_append(code, BC_Convert_Bool, NULL);
////                bytecode_append(code, BC_IfFalse_Jump, NULL + code->size + 3);
////                bytecode_append(code, BC_JumpBlock, block_temp);
////                bytecode_append(code, BC_Jump, NULL + pos);
//                break;
//            }
//            case StmtType_DoWhile: {
////                block_list_add_new(&parser->blocks);
////                struct block_expr_st *block_temp = block_list_last(&parser->blocks);
////                block_temp->sub_type = ScopeType_Loop;
////                block_temp->node = node->nodes.nodes[0];
////
////                size_t pos = code->size;
////                bytecode_append(code, BC_Jump, NULL + code->size + 4);
////                run_make_bytecode(parser, node->nodes.nodes[1], code);
////                bytecode_append(code, BC_Convert_Bool, NULL);
////                bytecode_append(code, BC_IfFalse_Jump, NULL + code->size + 3);
////                bytecode_append(code, BC_JumpBlock, block_temp);
////                bytecode_append(code, BC_Jump, NULL + pos);
//                break;
//            }
//            case StmtType_Class:
//                printf("StmtType_Class\n");
//                break;
//            case StmtType_TryWith: {
//                block_list_add_new(&parser->blocks);
//                struct block_expr_st *block_temp_try = block_list_last(&parser->blocks);
//                block_temp_try->sub_type = ScopeType_Try;
//                block_temp_try->node = node->nodes.nodes[0];
//
//                block_list_add_new(&parser->blocks);
//                struct block_expr_st *block_temp_with = block_list_last(&parser->blocks);
//                block_temp_with->sub_type = ScopeType_Try;
//                block_temp_with->node = node->nodes.nodes[0];
//
//                bytecode_append(code, BC_SaveStack, NULL);
//                bytecode_append(code, BC_JumpBlock, block_temp_try);
//                bytecode_append(code, BC_Jump, NULL + code->size + 3);
//                bytecode_append(code, BC_LoadStack, NULL); // Saving last param
//                bytecode_append(code, BC_JumpBlock, block_temp_with);
//                break;
//            }
//            case StmtType_Func:
//            case StmtType_Annot:
//                for (size_t i = 0; i < node->nodes.size; i++)
//                    run_make_bytecode(parser, node->nodes.nodes[i], code);
//                for (size_t i = 1; i < node->nodes.size; i++)
//                    bytecode_append(code, BC_Set, NULL);
//                bytecode_append(code, BC_Pop, NULL);
//                break;
//            case StmtType_List:
//                bytecode_append(code, BC_FrameInit, &node->variable->attrib);
//                for (size_t i = 0; i < node->nodes.size; i++)
//                    run_make_bytecode(parser, node->nodes.nodes[i], code);
//                bytecode_append(code, BC_FrameClose, NULL);
//                break;
//        }
//    }
//}
//
//void run_block(struct parser_st *parser, struct block_expr_st *block, size_t position) {
//    if (parser->interrupt_type != 0) {
////        if (parser->interrupt_type == Interrupt_Break && block->type == BlockType_Conf &&
////            block->sub_type == ScopeType_StmtLoop) {
////            parser->interrupt_type = 0;
////            return;
////        } else if (block->type != BlockType_OP || block->sub_type != Block_FrameClose) return;
//    }
//    struct block_list_st *blocks_stack = &parser->blocks_stack;
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
//        if(parser->interrupt_type == Interrupt_Break_Out) {
//            pos ++;
//            parser->interrupt_type = 0;
//        }
//        while (pos < code->size) {
//            char command = code->command[pos];
//            void *data = code->data[pos];
//            if (parser->interrupt_type == 0 || (command & 0xF0) == BC_Frame) {
//                if ((command & 0xF0) == 0) {
//                    switch (command) {
//                        case BC_Init:
//                            if (((struct attrib_st *) data)->data != NULL)
//                                object_free(((struct attrib_st *) data)->data);
//                            ((struct attrib_st *) data)->data = object_new();
//                        case BC_Load:
////                            print_obj(((struct attrib_st *) data)->data, 0);
//                            list_append(parser->temp_memory, ((struct attrib_st *) data)->data);
//                            break;
//                        case BC_LoadConst:
//                            list_append(parser->temp_memory, data);
//                            break;
//                        case BC_Attrib: {
//                            struct object_st *obj = list_pop(parser->temp_memory);
//                            struct object_st *err = object_new();
//                            struct object_st *res = object_attrib(err, obj, ((struct object_st *)data)->data);
//                            if (err->type != NONE_TYPE) {
//                                parser->interrupt_type = Interrupt_Throw;
//                                parser->interrupt_scopes = 0;
//                                list_append(parser->temp_memory, err);
//                            } else {
//                                list_append(parser->temp_memory, res);
//                            }
//                            object_free(res);
//                            object_free(err);
//                            object_free(obj);
//                            break;
//                        }
//                        case BC_Subscript: {
//                            struct object_st *obj1 = list_pop(parser->temp_memory);
//                            struct object_st *obj2 = list_pop(parser->temp_memory);
//                            struct object_st *err = object_new();
//                            struct object_st *res = object_subscript(err, obj1, obj2);
//                            if (err->type != NONE_TYPE) {
//                                parser->interrupt_type = Interrupt_Throw;
//                                parser->interrupt_scopes = 0;
//                                list_append(parser->temp_memory, err);
//                            } else {
//                                list_append(parser->temp_memory, res);
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
//                            struct object_st *obj = list_pop(parser->temp_memory);
//
//                            if (obj->type != OP_OBJECT_TYPE) {
//                                // TODO error
//                                exit(1);
//                            }
//                            struct op_object *func = obj->data;
//                            {
//
//                                frame_save_data(frame_list_add_new(&parser->frame_stack), &func->argument->attrib);
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
//                                temp = list_pop(parser->temp_memory);
//                                object_set(func->argument->attrib.attribs[i]->data, temp);
//                                object_free(temp);
//                            }
//
//                            block_list_append(blocks_stack, block, pos + 1);
//                            block_list_append(blocks_stack, func->function_body, 0);
//                            return;
//                        }
//                        case BC_Pop:
//                            object_free(list_pop(parser->temp_memory));
//                            break;
//                    }
//                } // Not // Done
//                else if ((command & 0xF0) == BC_Convert) {
//                    struct object_st *temp = NULL;
//                    struct object_st *obj = list_pop(parser->temp_memory);
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
//                        parser->interrupt_type = Interrupt_Throw;
//                        parser->interrupt_scopes = 0;
//                        list_append(parser->temp_memory, err);
//                    } else {
//                        list_append(parser->temp_memory, res);
//                    }
//                    object_free(res);
//                    object_free(err);
//
//                    object_free(obj);
//                    object_free(temp);
//                } // Done
//                else if ((command & 0xF0) == BC_Operations) {
//                    struct object_st *obj2 = list_pop(parser->temp_memory);
//                    struct object_st *res = object_new();
//                    struct object_st *err = object_new();
//                    if (command == BC_Negate) {
//                        object__neg(res, err, obj2);
//                        if (err->type != NONE_TYPE) {
//                            parser->interrupt_type = Interrupt_Throw;
//                            parser->interrupt_scopes = 0;
//                            list_append(parser->temp_memory, err);
//                        } else {
//                            list_append(parser->temp_memory, res);
//                        }
//                    } else if (command == BC_NegateBool) {
//                        object_set_type(res, INTEGER_TYPE);
//                        if (integer_is_null(obj2->data)) integer_set_ui(res->data, 1);
//                        else integer_set_ui(res->data, 0);
//                        list_append(parser->temp_memory, res);
//                    } else {
//                        struct object_st *obj1 = list_pop(parser->temp_memory);
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
//                                    list_append(parser->temp_memory, res);
//                                    break;
//                                }
//                                if (err->type != NONE_TYPE) {
//                                    parser->interrupt_type = Interrupt_Throw;
//                                    parser->interrupt_scopes = 0;
//                                    list_append(parser->temp_memory, err);
//                                } else list_append(parser->temp_memory, res);
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
//                                    parser->interrupt_type = Interrupt_Throw;
//                                    parser->interrupt_scopes = 0;
//                                    list_append(parser->temp_memory, err);
//                                } else {
//                                    object_set(obj1, res);
//                                    list_append(parser->temp_memory, obj1);
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
//                                list_append(parser->temp_memory, res);
//                                break;
//                            case BC_Set:
//                                object_set(obj1, obj2);
//                                list_append(parser->temp_memory, obj2);
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
//                        struct object_st *obj = list_pop(parser->temp_memory);
//                        size_t size; // TODO load stack size
//                        list_resize(parser->temp_memory, size);
//                        list_append(parser->temp_memory, obj);
//                        object_free(obj);
//                    }
//                    else {
////                        struct object_st *obj = list_pop(parser->temp_memory);
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
//                            block_list_add_new(&parser->blocks);
//                            block_temp = block_list_last(&parser->blocks);
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
//                                temp = list_pop(parser->temp_memory);
//                                list_append(obj->data, temp);
//                                object_free(temp);
//                            }
//                            break;
//                    }
//                    list_append(parser->temp_memory, obj);
//                    object_free(obj);
//                } // Not // Done
//                else if ((command & 0xF0) == BC_Frame) {
//                    if (command == BC_FrameInit && parser->interrupt_type == 0) {
//                        if (parser->interrupt_type != 0) parser->interrupt_scopes++;
//                        else frame_save_data(frame_list_add_new(&parser->frame_stack), data);
//                    } else
//                    if (command == BC_FrameClose) {
//                        if (parser->interrupt_type != 0 && parser->interrupt_scopes != 0) parser->interrupt_scopes --;
//                        else frame_load_data(frame_list_pop(&parser->frame_stack));
//                    } else
//                    if (command == BC_FrameCloseFunc && parser->interrupt_type == 0) {
//                        frame_load_data(frame_list_pop(&parser->frame_stack));
//                    }
//                }// Done
//                else if ((command & 0xF0) == BC_Interrupts) {
//                    if (command == BC_Break) parser->interrupt_type = Interrupt_Break;
//                    else if (command == BC_Throw) parser->interrupt_type = Interrupt_Throw;
//                    else if (command == BC_Return) parser->interrupt_type = Interrupt_Return;
//                    else if (command == BC_Continue) parser->interrupt_type = Interrupt_Continue;
//                    parser->interrupt_scopes = 0;
//                } // Not // Done
//            }
//            ++pos;
//        }
//        if(block->sub_type == ScopeType_Func && parser->interrupt_type == Interrupt_Return) parser->interrupt_type = 0;
//        if(block->sub_type == ScopeType_Loop && parser->interrupt_type == Interrupt_Continue) parser->interrupt_type = 0;
//        if(block->sub_type == ScopeType_Loop && parser->interrupt_type == Interrupt_Break) parser->interrupt_type = Interrupt_Break_Out;
//        if(block->sub_type == ScopeType_Try && parser->interrupt_type == Interrupt_Throw) parser->interrupt_type = 0;
//    }
//}
//
//void run_smart_contract(struct parser_st *parser, struct node_st *node) {
//    struct block_expr_st *block = NULL;
//    struct block_list_st *blocks_stack = &parser->blocks_stack;
//
//    {
//        block_list_add_new(&parser->blocks);
//        block = block_list_last(&parser->blocks);
//        block->node = node;
//        block_list_append(blocks_stack, block, 0);
//    }
//
//    while (blocks_stack->size) {
//        block = block_list_last(blocks_stack);
//        size_t position = block_list_last_pos(blocks_stack);
//        block_list_resize(blocks_stack, blocks_stack->size - 1);
//
//        run_block(parser, block, position);
//    }
//
//    print_list(parser->temp_memory, 0);
//}