#ifndef SC_STRUCTS_H
#define SC_STRUCTS_H

#include "struct.h"

#include "sc_structs/sc_parser.h"
#include "sc_structs/sm_state.h"
#include "sc_structs/op_state.h"

#include "sc_structs/tk_token.h"
#include "sc_structs/ast_node.h"
#include "sc_structs/op_block.h"
#include "sc_structs/op_object.h"
#include "sc_structs/op_attrib.h"


#define TK_TOKEN_SIZE       sizeof(struct tk_token)
#define AST_NODE_SIZE       sizeof(struct ast_node)
#define OP_BLOCK_SIZE       sizeof(struct ast_node)
#define OP_OBJECT_SIZE      sizeof(struct op_object)
#define OP_ATTRIB_SIZE      sizeof(struct op_attrib)


#define TK_TOKEN_OP     (struct object_op) {METHOD_NEW &tk_token_new, METHOD_SET &tk_token_set, METHOD_CLEAR &tk_token_clear, METHOD_FREE &tk_token_free, NULL}
#define AST_NODE_OP     (struct object_op) {METHOD_NEW &ast_node_new, METHOD_SET &ast_node_set, METHOD_CLEAR &ast_node_clear, METHOD_FREE &ast_node_free, NULL}
#define OP_BLOCK_OP     (struct object_op) {METHOD_NEW &op_block_new, METHOD_SET &op_block_set, METHOD_CLEAR &op_block_clear, METHOD_FREE &op_block_free, NULL}
#define OP_OBJECT_OP    (struct object_op) {METHOD_NEW &op_object_new, METHOD_SET &op_object_set, METHOD_CLEAR &op_object_clear, METHOD_FREE &op_object_free, NULL}
#define OP_ATTRIB_OP    (struct object_op) {METHOD_NEW &op_attrib_new, METHOD_SET &op_attrib_set, METHOD_CLEAR &op_attrib_clear, METHOD_FREE &op_attrib_free, NULL}


extern struct object_type tk_token_type;
extern struct object_type ast_node_type;
extern struct object_type op_block_type;
extern struct object_type op_object_type;
extern struct object_type op_attrib_type;


#define TK_TOKEN_TYPE       &tk_token_type
#define AST_NODE_TYPE       &ast_node_type
#define OP_BLOCK_TYPE       &op_block_type
#define OP_OBJECT_TYPE      &op_object_type
#define OP_ATTRIB_TYPE      &op_attrib_type


#endif //SC_STRUCTS_H
