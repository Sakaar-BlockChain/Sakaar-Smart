//#include <stdio.h>
//#include "sm_semantic.h"
//
//
//void sm_semantic_code_expr(struct parser_st *parser, struct node_st *node) {
//    switch (node->sub_type) {
//        case PrimType_List:
//            for (size_t i = node->nodes.size; i > 0; i--) {
//                sm_semantic_code(parser, node->nodes.nodes[i - 1], code);
//            }
//            bytecode_append(code, BC_MakeList, NULL + node->nodes.size);
//            break;
//        case PrimType_Ident_get:
//            bytecode_append(code, BC_Load, NULL + node->data);
//            break;
//        case PrimType_Ident_new:
//            bytecode_append(code, BC_Init, NULL + node->data);
//            break;
//        case PrimType_Literal:
//            bytecode_append(code, BC_LoadConst, NULL + node->data);
//            break;
//        case PrimType_Attrib:
//            sm_semantic_code(parser, node->nodes.nodes[0], code);
//            bytecode_append(code, BC_Attrib, NULL + node->data);
//            break;
//        case PrimType_Subscript:
//            sm_semantic_code(parser, node->nodes.nodes[1], code);
//            sm_semantic_code(parser, node->nodes.nodes[0], code);
//            bytecode_append(code, BC_Subscript, NULL);
//            break;
//        case PrimType_Call: {
//            size_t count = node->nodes.nodes[1]->nodes.size;
//            for (size_t i = count; i > 0; i--) {
//                sm_semantic_code(parser, node->nodes.nodes[1]->nodes.nodes[i - 1], code);
//            }
//            sm_semantic_code(parser, node->nodes.nodes[0], code);
//            if (node->nodes.nodes[0]->type == MainType_Expr && node->nodes.nodes[0]->sub_type == PrimType_Attrib) {
//                sm_semantic_code(parser, node->nodes.nodes[0]->nodes.nodes[1], code);
//            }
//            bytecode_append(code, BC_Call, NULL + count);
//            bytecode_append(code, BC_FrameCloseFunc, NULL);
//            break;
//        }
//    }
//}
//
//void sm_semantic_code_oper(struct parser_st *parser, struct node_st *node) {
//    for (size_t i = 0; i < node->nodes.size; i++) {
//        sm_semantic_code(parser, node->nodes.nodes[i]);
//    }
//}
//
//void sm_semantic_code_stmt(struct parser_st *parser, struct node_st *node) {
//    switch (node->sub_type) {
//        case StmtType_Assign:
//            break;
//        case StmtType_Return:
//            break;
//        case StmtType_Break:
//            break;
//        case StmtType_Continue:
//            break;
//        case StmtType_Oper:
//            break;
//        case StmtType_Func_Body:
//            break;
//        case StmtType_If:
//            break;
//        case StmtType_While:
//            break;
//        case StmtType_DoWhile:
//            break;
//        case StmtType_TryWith:
//            break;
//        case StmtType_Func:
//            break;
//        case StmtType_Annot:
//            break;
//        case StmtType_List:
//            break;
//    }
//}
//
//void sm_semantic_code(struct parser_st *parser, struct node_st *node) {
//    if (node->type == MainType_Expr) sm_semantic_code_expr(parser, node, code);
//    else if (node->type == MainType_Oper) sm_semantic_code_oper(parser, node, code);
//    else if (node->type == MainType_Stmt) sm_semantic_code_stmt(parser, node, code);
//}
////// Stack---------------For Functions
//////      SomeData
//////      SomeData
//////      Argument 1
//////      Argument 2
//////      Argument 3
//////      BackPoint -> BackPoint
//////      SomeData in function
//////      SomeData in function
////
////void