#ifndef OP_OPERATIONS_H
#define OP_OPERATIONS_H

#include "sn_syntax.h"
#include "operation.h"

void run_smart_contract(struct parser_st *);

void sc_first_run(struct parser_st *parser);

struct object_st *sc_run_function(struct parser_st *, struct object_st *, struct list_st *);

#endif //OP_OPERATIONS_H
