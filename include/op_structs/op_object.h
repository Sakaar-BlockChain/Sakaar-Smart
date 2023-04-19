#ifndef OP_OBJECT_H
#define OP_OBJECT_H

#include "sc_structs.h"

struct op_object{
    struct object_st *data;

    // Class Object
    struct object_st *class;
    struct map_st *attr;

    // Function inside
    struct frame_st *closure;
    struct variable_st *argument;
    struct block_expr_st *function_body;

    // Function outside
    void (*func)(struct list_st *, struct object_st *, struct op_object *);
};

struct op_object *op_object_new();
void op_object_set(struct op_object *res, const struct op_object *a);
void op_object_clear(struct op_object *res);
void op_object_free(struct op_object *res);

void op_object_set_data(struct op_object *, struct object_st *);
void op_object_set_function(struct op_object *res, struct variable_st *, struct closure_st *, struct block_expr_st *);
struct object_st *op_object_get_attrib(struct op_object *, struct string_st *);
struct object_st *op_object_set_attrib(struct op_object *, struct string_st *);

#endif //OP_OBJECT_H
