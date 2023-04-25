#ifndef AST_NODE_H
#define AST_NODE_H

#include "struct.h"
#include "token_list.h"
#include "node_list.h"

#define ExprType_None           0x00

#define MainType_None           0x00
#define MainType_Expr           0x01
#define MainType_Oper           0x02
#define MainType_Stmt           0x03

struct node_st{
    short type;
    short sub_type;

    struct token_list_st tokens;
    struct node_list_st nodes;

    struct object_st *data;
    struct variable_list_st *variable;
    struct closure_st *closure;

    size_t attrib;
};

struct node_st *node_new();
void node_set(struct node_st *, const struct node_st *);
void node_clear(struct node_st *);
void node_free(struct node_st *);

void node_data_init(struct node_st *);
void node_data_free(struct node_st *);


void print_node(const struct node_st *, int);

#endif //AST_NODE_H
