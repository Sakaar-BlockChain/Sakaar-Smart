#ifndef SM_STATE_H
#define SM_STATE_H

#include "ast_node.h"
#include "struct.h"


#define ScopeType_None  0x00
#define ScopeType_Func  0x01
#define ScopeType_Class 0x02
#define ScopeType_Loop  0x04


struct sm_state {
    struct list_st *memory_frame;
    struct list_st *memory_closure;
    struct list_st *expr_stack;

    int type;
    int error;
};

struct sm_state *sm_state_new();
void sm_state_free(struct sm_state *);

void sm_state_save_type(struct sm_state *res, struct ast_node *obj);
void sm_state_remove_frame(struct sm_state *res);

struct object_st *sm_state_set_ident(struct sm_state *, struct object_st *);
struct object_st *sm_state_get_ident(struct sm_state *, struct object_st *);





#endif //SM_STATE_H
