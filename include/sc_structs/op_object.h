//
// Created by 04024 on 06.04.2023.
//

#ifndef OP_OBJECT_H
#define OP_OBJECT_H

#include "struct.h"

struct op_object{
    struct string_st *name;

    struct object_st *data;

    struct object_st *class;
    struct list_st *attr;

    struct object_st *args;
    struct object_st *body;
};

struct op_object *op_object_new();
void op_object_set(struct op_object *res, const struct op_object *a);
void op_object_clear(struct op_object *res);
void op_object_free(struct op_object *res);

void op_object_set_name(struct op_object *res, const struct string_st *name);
void op_object_set_data(struct op_object *res, struct object_st *obj);


#endif //OP_OBJECT_H
