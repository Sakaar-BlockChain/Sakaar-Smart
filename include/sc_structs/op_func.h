#ifndef OP_FUNC_H
#define OP_FUNC_H

#include "ast_node.h"

struct op_func{
    struct object_st *args;
    struct object_st *body;
};

struct op_func *op_func_new();
void op_func_set(struct op_func *, const struct op_func *);
void op_func_clear(struct op_func *);
void op_func_free(struct op_func *);


#endif //OP_FUNC_H
