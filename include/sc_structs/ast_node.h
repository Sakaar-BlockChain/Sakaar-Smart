#ifndef AST_NODE_H
#define AST_NODE_H

#include "struct.h"

#define ExprType_None           0x00

#define MainType_None           0x00
#define MainType_Expr           0x01
#define MainType_Oper           0x02
#define MainType_Stmt           0x03

struct ast_node{
    short main_type;
    short type;

    struct object_st *data;
    struct list_st *next;
    struct list_st *tokens;
};


struct ast_node *ast_node_new();
void ast_node_set(struct ast_node *, const struct ast_node *);
void ast_node_clear(struct ast_node *);
void ast_node_free(struct ast_node *);

#endif //AST_NODE_H
