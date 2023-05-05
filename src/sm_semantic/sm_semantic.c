#include <stdio.h>
#include "sm_semantic.h"


void sm_semantic_expr(struct parser_st *parser, struct node_st *node) {
    
}

void sm_semantic_oper(struct parser_st *parser, struct node_st *node){
    
}

void sm_semantic_stmt(struct parser_st *parser, struct node_st *node){
    
}

void sm_semantic(struct parser_st *parser, struct node_st *node) {
    if (node->type == MainType_Expr) sm_semantic_expr(parser, node);
    else if (node->type == MainType_Oper) sm_semantic_oper(parser, node);
    else if (node->type == MainType_Stmt) sm_semantic_stmt(parser, node);
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