#ifndef ATTRIB_H
#define ATTRIB_H

#include "struct.h"

struct attrib_st{
    struct string_st name;
    struct object_st *data;

    short counter;
};

struct attrib_st *attrib_new();
void attrib_set(struct attrib_st *, const struct attrib_st *);
void attrib_clear(struct attrib_st *);
void attrib_free(struct attrib_st *);

struct attrib_st *attrib_copy(struct attrib_st *);

#endif //ATTRIB_H
