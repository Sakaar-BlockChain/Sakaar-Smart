#ifndef OP_CLASS_H
#define OP_CLASS_H

#include "sc_structs.h"

struct op_class {
    // Class Data
    struct map_st *attr;

    struct frame_st *closure;
    struct variable_list_st *argument;
    size_t class_body;
};

struct op_class *op_class_new();
void op_class_free(struct op_class *);

void op_class_set(struct op_class *, const struct op_class *);
void op_class_copy(struct op_class *, const struct op_class *);

void op_class_clear(struct op_class *);
int op_class_cmp(const struct op_class *, const struct op_class *);

void op_class_define(struct op_class *, struct node_st *, struct parser_st *);
struct object_st *op_class_save_value(struct op_class *, struct parser_st *);

// Sub method
struct object_st *op_class_subscript(struct object_st *, struct op_class *, const struct object_st *);
struct object_st *op_class_attrib(struct object_st *, const struct op_class *, const struct string_st *);

// Convert Methods
//void op_object__bool(struct object_st *, struct object_st *, const struct op_object *);
//void op_object__int(struct object_st *, struct object_st *, const struct op_object *);
//void op_object__float(struct object_st *, struct object_st *, const struct op_object *);
//void op_object__str(struct object_st *, struct object_st *, const struct op_object *);


#endif //OP_CLASS_H
