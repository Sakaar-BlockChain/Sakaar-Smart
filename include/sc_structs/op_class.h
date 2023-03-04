//
// Created by 04024 on 02.03.2023.
//

#ifndef OP_CLASS_H
#define OP_CLASS_H

#include "op_func.h"
#include "struct.h"

struct op_class {
    struct object_st *init;
    struct map_st *data;
};

struct op_class *op_class_new();
void op_class_set(struct op_class *, const struct op_class *);
void op_class_clear(struct op_class *);
void op_class_free(struct op_class *);


struct op_class_obj {
    struct object_st *class;
    struct map_st *data;
};


struct op_class_obj *op_class_obj_new();
void op_class_obj_set(struct op_class_obj *, const struct op_class_obj *);
void op_class_obj_clear(struct op_class_obj *);
void op_class_obj_free(struct op_class_obj *);

void op_class_object_init(struct op_class_obj *, struct object_st *);


#endif //OP_CLASS_H
