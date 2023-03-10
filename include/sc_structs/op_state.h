#ifndef OP_STATE_H
#define OP_STATE_H

#include "struct.h"

#define FLAG_NONE       0x00
#define FLAG_BREAK      0x01
#define FLAG_CONTINUE   0x02
#define FLAG_RETURN     0x03

struct op_state{
    struct object_st *error_obj;
    struct object_st *return_obj;

    short flag;

    struct stack_st *memory;
    struct stack_st *temp_memory;
    struct stack_st *code_operations;
};

struct op_state *op_state_new();
void op_state_set(struct op_state *, const struct op_state *);
void op_state_clear(struct op_state *);
void op_state_free(struct op_state *);

void op_state_append(struct op_state *);
void op_state_remove(struct op_state *);

#endif //OP_STATE_H
