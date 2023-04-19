#ifndef CLOSURE_H
#define CLOSURE_H

#include "struct.h"
#include "attrib_list.h"

struct closure_st{
    struct attrib_list_st attrib;
    struct attrib_list_st data;
};

struct closure_st *closure_new();
void closure_set(struct closure_st *, const struct closure_st *);
void closure_clear(struct closure_st *);
void closure_free(struct closure_st *);

void closure_data_init(struct closure_st *);
void closure_data_free(struct closure_st *);

void closure_append(struct closure_st *, struct attrib_st *, struct attrib_st *);

void print_closure(const struct closure_st *, int);

#endif //CLOSURE_H
