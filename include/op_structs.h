#ifndef OP_STRUCTS_H
#define OP_STRUCTS_H

#include "sc_structs.h"

#include "op_structs/op_class.h"
#include "op_structs/op_function.h"
#include "op_structs/op_object.h"


#define OP_CLASS_SIZE       sizeof(struct op_class)
#define OP_FUNCTION_SIZE    sizeof(struct op_function)
#define OP_OBJECT_SIZE      sizeof(struct op_object)


#define OP_CLASS_OP     {METHOD_NEW &op_class_new, METHOD_FREE &op_class_free, METHOD_SET &op_class_set, METHOD_SET &op_class_copy, METHOD_CLEAR &op_class_clear, METHOD_CMP &op_class_cmp}
#define OP_FUNCTION_OP  {METHOD_NEW &op_function_new, METHOD_FREE &op_function_free, METHOD_SET &op_function_set, METHOD_SET &op_function_copy, METHOD_CLEAR &op_function_clear, METHOD_CMP &op_function_cmp}
#define OP_OBJECT_OP    {METHOD_NEW &op_object_new, METHOD_FREE &op_object_free, METHOD_SET &op_object_set, METHOD_SET &op_object_copy, METHOD_CLEAR &op_object_clear, METHOD_CMP &op_object_cmp}


extern struct object_type op_class_type;
extern struct object_type op_function_type;
extern struct object_type op_object_type;


#define OP_CLASS_TYPE       &op_class_type
#define OP_FUNCTION_TYPE    &op_function_type
#define OP_OBJECT_TYPE      &op_object_type

#endif //OP_STRUCTS_H
