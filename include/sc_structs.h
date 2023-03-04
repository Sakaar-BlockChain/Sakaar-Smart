#ifndef SC_STRUCTS_H
#define SC_STRUCTS_H

#include "struct.h"

#include "sc_structs/sc_parser.h"

#include "sc_structs/tk_token.h"
#include "sc_structs/ast_node.h"
#include "sc_structs/op_block.h"
#include "sc_structs/op_func.h"
#include "sc_structs/op_class.h"

#include "sc_structs/op_state.h"


#define TK_TOKEN_SIZE       sizeof(struct tk_token)
#define AST_NODE_SIZE       sizeof(struct ast_node)
#define OP_BLOCK_SIZE       sizeof(struct ast_node)
#define OP_FUNC_SIZE        sizeof(struct op_func)
#define OP_CLASS_SIZE       sizeof(struct op_class)
#define OP_CLASS_OBJ_SIZE   sizeof(struct op_class_obj)


#define TK_TOKEN_OP     (struct object_op) {METHOD_NEW &tk_token_new, METHOD_SET &tk_token_set, METHOD_CLEAR &tk_token_clear, METHOD_FREE &tk_token_free, NULL}
#define AST_NODE_OP     (struct object_op) {METHOD_NEW &ast_node_new, METHOD_SET &ast_node_set, METHOD_CLEAR &ast_node_clear, METHOD_FREE &ast_node_free, NULL}
#define OP_BLOCK_OP     (struct object_op) {METHOD_NEW &op_block_new, METHOD_SET &op_block_set, METHOD_CLEAR &op_block_clear, METHOD_FREE &op_block_free, NULL}
#define OP_FUNC_OP      (struct object_op) {METHOD_NEW &op_func_new, METHOD_SET &op_func_set, METHOD_CLEAR &op_func_clear, METHOD_FREE &op_func_free, NULL}
#define OP_CLASS_OP     (struct object_op) {METHOD_NEW &op_class_new, METHOD_SET &op_class_set, METHOD_CLEAR &op_class_clear, METHOD_FREE &op_class_free, NULL}
#define OP_CLASS_OBJ_OP (struct object_op) {METHOD_NEW &op_class_obj_new, METHOD_SET &op_class_obj_set, METHOD_CLEAR &op_class_obj_clear, METHOD_FREE &op_class_obj_free, NULL}


extern struct object_type tk_token_type;
extern struct object_type ast_node_type;
extern struct object_type op_block_type;
extern struct object_type op_func_type;
extern struct object_type op_class_type;
extern struct object_type op_class_obj_type;


#define TK_TOKEN_TYPE       &tk_token_type
#define AST_NODE_TYPE       &ast_node_type
#define OP_BLOCK_TYPE       &op_block_type
#define OP_FUNC_TYPE        &op_func_type
#define OP_CLASS_TYPE       &op_class_type
#define OP_CLASS_OBJ_TYPE   &op_class_obj_type


#endif //SC_STRUCTS_H
