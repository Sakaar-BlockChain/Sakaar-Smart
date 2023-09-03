#include "cg_code.h"

size_t cg_generate_code_function(struct parser_st *parser, struct node_st *node) {
    size_t result = parser_codespace(parser);
    struct bytecode_st *code = parser->codes.bytecodes[result];
    code->closure = node->closure;
    code->variable = node->variable;
    
    cg_generate_code(parser, node->nodes.nodes[1], code);
    for (size_t i = 0, size = code->size; i < size; i++) {
        if (code->command[i] == BC_Return && code->data[i] == 0) {
            code->command[i] = BC_Jump;
            code->data[i] = code->size;
        }
    }
    bytecode_append(code, BC_FuncEnd, 0);
    return result;
}

size_t cg_generate_code_class(struct parser_st *parser, struct node_st *node) {
    size_t result = parser_codespace(parser);
    struct bytecode_st *code = parser->codes.bytecodes[result];
    code->closure = node->closure;
    code->variable = node->variable;

    if (node->nodes.size == 2) {
        cg_generate_code(parser, node->nodes.nodes[1], code);
    } else {
        cg_generate_code(parser, node->nodes.nodes[0], code);
    }
    bytecode_append(code, BC_ClassEnd, 0);
    return result;
}

void cg_generate_code_expr(struct parser_st *parser, struct node_st *node, struct bytecode_st *code) {
    switch (node->sub_type) {
        case PrimType_List:
            for (size_t i = node->nodes.size; i > 0; i--) {
                cg_generate_code(parser, node->nodes.nodes[i - 1], code);
            }
            bytecode_append(code, BC_MakeList, node->nodes.size);
            break;
        case PrimType_Ident_get:
            bytecode_append(code, BC_Load, node->data);
            break;
        case PrimType_Ident_new:
            bytecode_append(code, BC_Init, node->data);
            break;
        case PrimType_Literal:
            bytecode_append(code, BC_LoadConst, node->data);
            break;
        case PrimType_Attrib:
            cg_generate_code(parser, node->nodes.nodes[0], code);
            bytecode_append(code, BC_Attrib_Del, node->data);
            break;
        case PrimType_Subscript:
            cg_generate_code(parser, node->nodes.nodes[1], code);
            cg_generate_code(parser, node->nodes.nodes[0], code);
            bytecode_append(code, BC_Subscript, 0);
            break;
        case PrimType_Call: {
            size_t count = node->nodes.nodes[1]->nodes.size;
            for (size_t i = count; i > 0; i--) {
                cg_generate_code(parser, node->nodes.nodes[1]->nodes.nodes[i - 1], code);
            }
            cg_generate_code(parser, node->nodes.nodes[0], code);
            if (node->nodes.nodes[0]->type == MainType_Expr && node->nodes.nodes[0]->sub_type == PrimType_Attrib) {
                code->command[code->size - 1] = BC_Attrib;
                ++count;
            }
            bytecode_append(code, BC_Call, count);
            break;
        }
    }
}

void cg_generate_code_oper(struct parser_st *parser, struct node_st *node, struct bytecode_st *code) {
    if (node->sub_type == ExprType_OrTest || node->sub_type == ExprType_AndTest) {
        char type = BC_IfFalseOrPop;
        size_t *array = malloc(sizeof(size_t) * (node->nodes.size - 1));
        if (node->sub_type == ExprType_OrTest) type = BC_IfTrueOrPop;
        for (size_t i = 0, size = node->nodes.size - 1; i < size; i++) {
            cg_generate_code(parser, node->nodes.nodes[i], code);
            if (node->nodes.nodes[i]->type != MainType_Oper || node->nodes.nodes[i]->sub_type < ExprType_Comp)
                bytecode_append(code, BC_Convert_Bool, 0);
            bytecode_append(code, type, 0);
            array[i] = code->size - 1;
        }

        cg_generate_code(parser, node->nodes.nodes[node->nodes.size - 1], code);
        if (node->nodes.nodes[node->nodes.size - 1]->type != MainType_Oper ||
            node->nodes.nodes[node->nodes.size - 1]->sub_type < ExprType_Comp)
            bytecode_append(code, BC_Convert_Bool, 0);

        for (size_t i = 0, size = node->nodes.size - 1; i < size; i++) {
            code->data[array[i]] = code->size;
        }
        free(array);
        return;
    }
    size_t count = 2;
    int is_bool = 0;
    if (node->sub_type == ExprType_U || node->sub_type == ExprType_NotTest) count = 1;
    if (node->sub_type == ExprType_AndTest || node->sub_type == ExprType_NotTest ||
        node->sub_type == ExprType_OrTest)
        is_bool = 1;


    for (size_t i = 0, size = node->nodes.size; i < size; i++) {
        cg_generate_code(parser, node->nodes.nodes[i], code);
        if (is_bool && (node->nodes.nodes[i]->type != MainType_Oper || node->nodes.nodes[i]->sub_type < ExprType_Comp))
            bytecode_append(code, BC_Convert_Bool, 0);
    }

    for (size_t i = node->tokens.size; i > 0; i--) {
        if (count == 2) {
            if (is_bool) bytecode_append(code, BC_Compare, node->tokens.tokens[i - 1]->sub_type);
            else bytecode_append(code, BC_Arithmetic, node->tokens.tokens[i - 1]->sub_type);
        } else {
            if (is_bool) bytecode_append(code, BC_NegateBool, 0);
            else bytecode_append(code, BC_Negate, 0);
        }
    }
}

void cg_generate_code_stmt(struct parser_st *parser, struct node_st *node, struct bytecode_st *code) {
    switch (node->sub_type) {
        case StmtType_Assign: {
            for (size_t i = 0, size = node->nodes.size; i < size; i++)
                cg_generate_code(parser, node->nodes.nodes[i], code);
            for (size_t i = node->tokens.size; i > 0; i--)
                bytecode_append(code, BC_ArithmeticSet, node->tokens.tokens[i - 1]->sub_type);
            bytecode_append(code, BC_Pop, 0);
            break;
        }
        case StmtType_Return: {
            if (node->nodes.size != 0) cg_generate_code(parser, node->nodes.nodes[0], code);
            else bytecode_append(code, BC_LoadConst, 0);
            bytecode_append(code, BC_Return, 0);
            break;
        }
        case StmtType_Break: {
            bytecode_append(code, BC_Break, 0);
            break;
        }
        case StmtType_Continue: {
            bytecode_append(code, BC_Continue, 0);
            break;
        }
        case StmtType_Oper: {
            cg_generate_code(parser, node->nodes.nodes[0], code);
            bytecode_append(code, BC_Pop, 0);
            break;
        }
        case StmtType_If: {
            size_t *array = malloc(sizeof(size_t) * (node->nodes.size / 2));
            size_t pos;
            for (size_t i = 1, size = node->nodes.size; i < size; i += 2) {
                cg_generate_code(parser, node->nodes.nodes[i - 1], code);
                if (node->nodes.nodes[i - 1]->type != MainType_Oper || node->nodes.nodes[i- 1]->sub_type < ExprType_Comp)
                    bytecode_append(code, BC_Convert_Bool, 0);

                bytecode_append(code, BC_IfFalse_Jump, 0);
                pos = code->size - 1;
                cg_generate_code(parser, node->nodes.nodes[i], code);
                if ((node->nodes.size % 2 == 1) || i + 2 < node->nodes.size) {
                    bytecode_append(code, BC_Jump, 0);
                    array[i / 2] = code->size - 1;
                }
                code->data[pos] = code->size;
            }
            if (node->nodes.size % 2 == 1) {
                cg_generate_code(parser, node->nodes.nodes[node->nodes.size - 1], code);
            }
            for (size_t i = 1, size = node->nodes.size; i < size; i += 2) {
                if ((node->nodes.size % 2 == 1) || i + 2 < node->nodes.size) {
                    code->data[array[i / 2]] = code->size;
                }
            }
            free(array);
            break;
        }
        case StmtType_While: {
            size_t pos_fins, pos_start;

            pos_start = code->size;
            cg_generate_code(parser, node->nodes.nodes[0], code);
            if (node->nodes.nodes[0]->type != MainType_Oper || node->nodes.nodes[0]->sub_type < ExprType_Comp)
                bytecode_append(code, BC_Convert_Bool, 0);
            bytecode_append(code, BC_IfFalse_Jump, 0);
            pos_fins = code->size - 1;
            cg_generate_code(parser, node->nodes.nodes[1], code);
            bytecode_append(code, BC_Jump, pos_start);
            code->data[pos_fins] = code->size;
            for (size_t i = pos_start, size = code->size; i < size; i++) {
                if (code->command[i] == BC_Break && code->data[i] == 0) {
                    code->command[i] = BC_Jump;
                    code->data[i] = code->size;
                } else if (code->command[i] == BC_Continue && code->data[i] == 0) {
                    code->command[i] = BC_Jump;
                    code->data[i] = pos_start;
                }
            }
            break;
        }
        case StmtType_DoWhile: {
            size_t pos_fins, pos_start;

            bytecode_append(code, BC_Jump, 0);
            pos_fins = code->size - 1;
            pos_start = code->size;
            cg_generate_code(parser, node->nodes.nodes[1], code);
            if (node->nodes.nodes[1]->type != MainType_Oper || node->nodes.nodes[1]->sub_type < ExprType_Comp)
                bytecode_append(code, BC_Convert_Bool, 0);
            bytecode_append(code, BC_IfFalse_Jump, 0);
            code->data[pos_fins] = code->size;
            pos_fins = code->size - 1;
            cg_generate_code(parser, node->nodes.nodes[0], code);
            bytecode_append(code, BC_Jump, pos_start);
            code->data[pos_fins] = code->size;
            for (size_t i = pos_start, size = code->size; i < size; i++) {
                if (code->command[i] == BC_Break && code->data[i] == 0) {
                    code->command[i] = BC_Jump;
                    code->data[i] = code->size;
                } else if (code->command[i] == BC_Continue && code->data[i] == 0) {
                    code->command[i] = BC_Jump;
                    code->data[i] = pos_start;
                }
            }
            break;
        }
        case StmtType_TryWith: {
//            hash_time_list_add_new(&parser->blocks);
//            struct block_expr_st *block_temp_try = hash_time_list_last(&parser->blocks);
//            block_temp_try->sub_type = ScopeType_Try;
//            block_temp_try->node = node->nodes.nodes[0];
//
//            hash_time_list_add_new(&parser->blocks);
//            struct block_expr_st *block_temp_with = hash_time_list_last(&parser->blocks);
//            block_temp_with->sub_type = ScopeType_Try;
//            block_temp_with->node = node->nodes.nodes[0];
//
//            bytecode_append(code, BC_SaveStack, 0);
//            bytecode_append(code, BC_JumpBlock, block_temp_try);
//            bytecode_append(code, BC_Jump, code->size + 3);
//            bytecode_append(code, BC_LoadStack, 0); // Saving last param
//            bytecode_append(code, BC_JumpBlock, block_temp_with);
//            break;
        }
        case StmtType_Annot: {
            for (size_t i = 0, size = node->nodes.size; i < size; i++)
                cg_generate_code(parser, node->nodes.nodes[i], code);
            for (size_t i = node->nodes.size; i > 0; i--)
                if (node->nodes.nodes[i - 1]->type == MainType_Expr &&
                    node->nodes.nodes[i - 1]->sub_type == PrimType_Ident_new)
                    bytecode_append(code, BC_Pop, 0);
                else
                    bytecode_append(code, BC_Set, 0);
            break;
        }
        case StmtType_List: {
            for (size_t i = 0, size = node->nodes.size; i < size; i++)
                cg_generate_code(parser, node->nodes.nodes[i], code);
            break;
        }
        case StmtType_Func_Body: {
            bytecode_append(code, BC_MakeFunc, cg_generate_code_function(parser, node));
            break;
        }
        case StmtType_Func: {
            for (size_t i = 0, size = node->nodes.size; i < size; i++)
                cg_generate_code(parser, node->nodes.nodes[i], code);
            bytecode_append(code, BC_Set, 0);
            bytecode_append(code, BC_Pop, 0);
            break;
        }
        case StmtType_Class_Body: {
            bytecode_append(code, BC_MakeClass, cg_generate_code_class(parser, node));
            break;
        }
        case StmtType_Class: {
            for (size_t i = 0, size = node->nodes.size; i < size; i++)
                cg_generate_code(parser, node->nodes.nodes[i], code);
            bytecode_append(code, BC_Set, 0);
            bytecode_append(code, BC_Pop, 0);
            break;
        }
    }
}

void cg_generate_code(struct parser_st *parser, struct node_st *node, struct bytecode_st *code) {
    if (node->type == MainType_Expr) cg_generate_code_expr(parser, node, code);
    else if (node->type == MainType_Oper) cg_generate_code_oper(parser, node, code);
    else if (node->type == MainType_Stmt) cg_generate_code_stmt(parser, node, code);
}

void cg_generate(struct parser_st *parser) {
    size_t code = parser_codespace(parser);
    cg_generate_code(parser, parser->nodes.nodes[0], parser->codes.bytecodes[code]);
    node_list_clear(&parser->nodes);
    token_list_clear(&parser->tokens);
}
//// Stack---------------For Functions
////      SomeData
////      SomeData
////      Argument 1
////      Argument 2
////      Argument 3
////      BackPoint -> BackPoint
////      SomeData in function
////      SomeData in function
//
//void