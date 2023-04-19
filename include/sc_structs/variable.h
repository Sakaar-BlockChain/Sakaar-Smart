#ifndef VARIABLE_H
#define VARIABLE_H

#include "struct.h"
#include "attrib_list.h"

struct variable_st{
    struct attrib_list_st attrib;
};

struct variable_st *variable_new();
void variable_set(struct variable_st *, const struct variable_st *);
void variable_clear(struct variable_st *);
void variable_free(struct variable_st *);

void variable_data_init(struct variable_st *);
void variable_data_free(struct variable_st *);


void print_variable(const struct variable_st *, int);


#endif //VARIABLE_H
