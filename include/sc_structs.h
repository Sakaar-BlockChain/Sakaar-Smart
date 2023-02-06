#ifndef SC_STRUCTS_H
#define SC_STRUCTS_H

#include "sc_structs/tk_token.h"
#include "sc_structs/tk_parser.h"

#include "sc_structs/an_node.h"
#include "sc_structs/an_parser.h"

#include "sc_structs/op_state.h"
#include "sc_structs/op_block.h"


#define TK_TOKEN_SIZE       sizeof(struct tk_token)
#define AN_NODE_SIZE        sizeof(struct an_node)
#define OP_BLOCK_SIZE       sizeof(struct an_node)


#define TK_TOKEN_OP     (struct object_op) {METHOD_NEW &tk_token_new, METHOD_SET &tk_token_set, METHOD_CLEAR &tk_token_clear, METHOD_FREE &tk_token_free, NULL}
#define AN_NODE_OP      (struct object_op) {METHOD_NEW &an_node_new, METHOD_SET &an_node_set, METHOD_CLEAR &an_node_clear, METHOD_FREE &an_node_free, NULL}
#define OP_BLOCK_OP     (struct object_op) {METHOD_NEW &op_block_new, METHOD_SET &op_block_set, METHOD_CLEAR &op_block_clear, METHOD_FREE &op_block_free, NULL}


extern struct object_type tk_token_type;
extern struct object_type an_node_type;
extern struct object_type op_block_type;


#define TK_TOKEN_TYPE    &tk_token_type
#define AN_NODE_TYPE     &an_node_type
#define OP_BLOCK_TYPE    &op_block_type


#endif //SC_STRUCTS_H
