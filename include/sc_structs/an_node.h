#ifndef AN_NODE_H
#define AN_NODE_H

#include "struct.h"

#define ExprType_None           0x00

#define MainType_None           0x00
#define MainType_Expr           0x01
#define MainType_Oper           0x02
#define MainType_Stmt           0x03

struct an_node{
    short main_type;
    short type;

    struct object_st *data;
    struct list_st *next;
    struct list_st *tokens;
};


struct an_node *an_node_new();
void an_node_set(struct an_node *, const struct an_node *);
void an_node_clear(struct an_node *);
void an_node_free(struct an_node *);

#endif //AN_NODE_H
