#ifndef SM_STATE_H
#define SM_STATE_H

#include "sc_structs.h"

struct sm_state{
    struct stack_st *memory_stack;
    struct list_st *memory_list;
};

struct sm_state *sm_parser_new();
void sm_parser_set(struct sm_state *, const struct sm_state *);
void sm_parser_clear(struct sm_state *);
void sm_parser_free(struct sm_state *);





#endif //SM_STATE_H
