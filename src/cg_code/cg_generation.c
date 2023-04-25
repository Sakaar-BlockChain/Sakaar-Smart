#include <stdio.h>
#include "cg_code.h"


void cg_generate_code_expr(struct parser_st *parser, struct node_st *node, struct bytecode_st *code) {
    switch (node->sub_type) {
        case PrimType_List:
            for (size_t i = node->nodes.size; i > 0; i--) {
                cg_generate_code(parser, node->nodes.nodes[i - 1], code);
            }
            bytecode_append(code, BC_MakeList, NULL + node->nodes.size);
            break;
        case PrimType_Ident_get:
            bytecode_append(code, BC_Load, NULL + node->attrib);
            break;
        case PrimType_Ident_new:
            bytecode_append(code, BC_Init, NULL + node->attrib);
            break;
        case PrimType_Literal:
            bytecode_append(code, BC_LoadConst, node->data);
            break;
        case PrimType_Attrib:
            cg_generate_code(parser, node->nodes.nodes[0], code);
            bytecode_append(code, BC_Attrib, node->data);
            break;
        case PrimType_Subscript:
            cg_generate_code(parser, node->nodes.nodes[1], code);
            cg_generate_code(parser, node->nodes.nodes[0], code);
            bytecode_append(code, BC_Subscript, NULL);
            break;
        case PrimType_Call: {
            size_t count = node->nodes.nodes[1]->nodes.size;
            for (size_t i = count; i > 0; i--) {
                cg_generate_code(parser, node->nodes.nodes[1]->nodes.nodes[i - 1], code);
            }
            cg_generate_code(parser, node->nodes.nodes[0], code);
            if (node->nodes.nodes[0]->type == MainType_Expr && node->nodes.nodes[0]->sub_type == PrimType_Attrib) {
                cg_generate_code(parser, node->nodes.nodes[0]->nodes.nodes[1], code);
            }
            bytecode_append(code, BC_Call, NULL + count);
            bytecode_append(code, BC_FrameCloseFunc, NULL);
            break;
        }
    }
}

void cg_generate_code_oper(struct parser_st *parser, struct node_st *node, struct bytecode_st *code) {
    if (node->sub_type == ExprType_OrTest || node->sub_type == ExprType_AndTest) {
        char type = BC_IfFalseOrPop;
        size_t *array = skr_malloc(sizeof(size_t) * (node->nodes.size - 1));
        if (node->sub_type == ExprType_OrTest) type = BC_IfTrueOrPop;
        for (size_t i = 0; i < node->nodes.size - 1; i++) {
            cg_generate_code(parser, node->nodes.nodes[i], code);
            bytecode_append(code, BC_Convert_Bool, NULL);
            bytecode_append(code, type, NULL);
            array[i] = code->size - 1;
        }

        cg_generate_code(parser, node->nodes.nodes[node->nodes.size - 1], code);
        bytecode_append(code, BC_Convert_Bool, NULL);

        for (size_t i = 0; i < node->nodes.size - 1; i++) {
            code->data[array[i]] = NULL + code->size;
        }
        skr_free(array);
        return;
    }
    size_t count = 2;
    int is_bool = 0;
    if (node->sub_type == ExprType_U || node->sub_type == ExprType_NotTest) count = 1;
    if (node->sub_type == ExprType_AndTest || node->sub_type == ExprType_NotTest ||
        node->sub_type == ExprType_OrTest)
        is_bool = 1;


    for (size_t i = 0; i < node->nodes.size; i++) {
        cg_generate_code(parser, node->nodes.nodes[i], code);
        if (is_bool) bytecode_append(code, BC_Convert_Bool, NULL);
    }

    for (size_t i = node->tokens.size; i > 0; i--) {
        if (count == 2) {
            if (is_bool) bytecode_append(code, BC_Compare, NULL + node->tokens.tokens[i - 1]->sub_type);
            else bytecode_append(code, BC_Arithmetic, NULL + node->tokens.tokens[i - 1]->sub_type);
        } else {
            if (is_bool) bytecode_append(code, BC_NegateBool, NULL);
            else bytecode_append(code, BC_Negate, NULL);
        }
    }
}

void cg_generate_code_stmt(struct parser_st *parser, struct node_st *node, struct bytecode_st *code) {switch (node->sub_type) {
        case StmtType_Assign:
            for (size_t i = 0; i < node->nodes.size; i++)
                cg_generate_code(parser, node->nodes.nodes[i], code);
            for (size_t i = node->tokens.size; i > 0; i--)
                bytecode_append(code, BC_ArithmeticSet, NULL + node->tokens.tokens[i - 1]->sub_type);

            bytecode_append(code, BC_Pop, NULL);
            break;
        case StmtType_Return:
            if (node->nodes.size != 0) cg_generate_code(parser, node->nodes.nodes[0], code);
            bytecode_append(code, BC_Return, NULL);
            break;
        case StmtType_Break:
            bytecode_append(code, BC_Break, NULL);
            break;
        case StmtType_Continue:
            bytecode_append(code, BC_Continue, NULL);
            break;
        case StmtType_Oper:
            cg_generate_code(parser, node->nodes.nodes[0], code);
            bytecode_append(code, BC_Pop, NULL);
            break;
        case StmtType_Func_Body:
            bytecode_append(code, BC_MakeFunc, node);
            break;
        case StmtType_If: {
//                void ***array = skr_malloc(sizeof(void **) * (node->nodes.size / 2));
//                void **point = NULL;
//                for (size_t i = 1; i < node->nodes.size; i += 2) {
//                    block_list_add_new(&parser->blocks);
//                    struct block_expr_st *block_temp = block_list_last(&parser->blocks);
//                    block_temp->sub_type = ScopeType_None;
//                    block_temp->node = node->nodes.nodes[i];
//
//                    cg_generate_code(parser, node->nodes.nodes[i - 1], code);
//                    bytecode_append(code, BC_Convert_Bool, NULL);
//
//                    bytecode_append(code, BC_IfFalse_Jump, NULL);
//                    point = &code->data[code->size - 1];
//                    bytecode_append(code, BC_JumpBlock, block_temp);
//                    bytecode_append(code, BC_Jump, NULL);
//                    *point = NULL + code->size;
//                    array[i / 2] = &code->data[code->size - 1];
//                }
//                if (node->nodes.size % 2 == 1) {
//                    block_list_add_new(&parser->blocks);
//                    struct block_expr_st *block_temp = block_list_last(&parser->blocks);
//                    block_temp->sub_type = ScopeType_None;
//                    block_temp->node = node->nodes.nodes[node->nodes.size - 1];
//
//                    bytecode_append(code, BC_JumpBlock, block_temp);
//                }
//                for (size_t i = 1; i < node->nodes.size; i += 2) {
//                    *array[i / 2] = NULL + code->size;
//                }
//                skr_free(array);
            break;
        }
        case StmtType_While: {
//                block_list_add_new(&parser->blocks);
//                struct block_expr_st *block_temp = block_list_last(&parser->blocks);
//                block_temp->sub_type = ScopeType_Loop;
//                block_temp->node = node->nodes.nodes[1];
//
//                size_t pos = code->size;
//                cg_generate_code(parser, node->nodes.nodes[0], code);
//                bytecode_append(code, BC_Convert_Bool, NULL);
//                bytecode_append(code, BC_IfFalse_Jump, NULL + code->size + 3);
//                bytecode_append(code, BC_JumpBlock, block_temp);
//                bytecode_append(code, BC_Jump, NULL + pos);
            break;
        }
        case StmtType_DoWhile: {
//                block_list_add_new(&parser->blocks);
//                struct block_expr_st *block_temp = block_list_last(&parser->blocks);
//                block_temp->sub_type = ScopeType_Loop;
//                block_temp->node = node->nodes.nodes[0];
//
//                size_t pos = code->size;
//                bytecode_append(code, BC_Jump, NULL + code->size + 4);
//                cg_generate_code(parser, node->nodes.nodes[1], code);
//                bytecode_append(code, BC_Convert_Bool, NULL);
//                bytecode_append(code, BC_IfFalse_Jump, NULL + code->size + 3);
//                bytecode_append(code, BC_JumpBlock, block_temp);
//                bytecode_append(code, BC_Jump, NULL + pos);
            break;
        }
        case StmtType_TryWith: {
//            block_list_add_new(&parser->blocks);
//            struct block_expr_st *block_temp_try = block_list_last(&parser->blocks);
//            block_temp_try->sub_type = ScopeType_Try;
//            block_temp_try->node = node->nodes.nodes[0];
//
//            block_list_add_new(&parser->blocks);
//            struct block_expr_st *block_temp_with = block_list_last(&parser->blocks);
//            block_temp_with->sub_type = ScopeType_Try;
//            block_temp_with->node = node->nodes.nodes[0];
//
//            bytecode_append(code, BC_SaveStack, NULL);
//            bytecode_append(code, BC_JumpBlock, block_temp_try);
//            bytecode_append(code, BC_Jump, NULL + code->size + 3);
//            bytecode_append(code, BC_LoadStack, NULL); // Saving last param
//            bytecode_append(code, BC_JumpBlock, block_temp_with);
//            break;
        }
        case StmtType_Func:
        case StmtType_Annot:
            for (size_t i = 0; i < node->nodes.size; i++)
                cg_generate_code(parser, node->nodes.nodes[i], code);
            for (size_t i = 1; i < node->nodes.size; i++)
                bytecode_append(code, BC_Set, NULL);
            bytecode_append(code, BC_Pop, NULL);
            break;
        case StmtType_List:
            bytecode_append(code, BC_FrameInit, &node->variable);
            for (size_t i = 0; i < node->nodes.size; i++)
                cg_generate_code(parser, node->nodes.nodes[i], code);
            bytecode_append(code, BC_FrameClose, NULL);
            break;
    }
}

void cg_generate_code(struct parser_st *parser, struct node_st *node, struct bytecode_st *code) {
    if (node->type == MainType_Expr) cg_generate_code_expr(parser, node, code);
    else if (node->type == MainType_Oper) cg_generate_code_oper(parser, node, code);
    else if (node->type == MainType_Stmt) cg_generate_code_stmt(parser, node, code);
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