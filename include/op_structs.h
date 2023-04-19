#ifndef OP_STRUCTS_H
#define OP_STRUCTS_H

#include "sc_structs.h"

#include "op_structs/op_object.h"


#define OP_OBJECT_SIZE      sizeof(struct op_object)


#define OP_OBJECT_OP    (struct object_op) {METHOD_NEW &op_object_new, METHOD_SET &op_object_set, METHOD_CLEAR &op_object_clear, METHOD_FREE &op_object_free, NULL}


extern struct object_type op_object_type;


#define OP_OBJECT_TYPE      &op_object_type

#endif //OP_STRUCTS_H
