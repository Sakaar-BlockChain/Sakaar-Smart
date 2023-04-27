#ifndef BYTECODE_LIST_H
#define BYTECODE_LIST_H

#include "struct.h"

struct bytecode_list_st{
    struct bytecode_st** bytecodes;
    size_t size;
    size_t max_size;
    int type;
};

void bytecode_list_set(struct bytecode_list_st *, const struct bytecode_list_st *);
void bytecode_list_clear(struct bytecode_list_st *);

void bytecode_list_data_init(struct bytecode_list_st *);
void bytecode_list_data_free(struct bytecode_list_st *);

void bytecode_list_resize(struct bytecode_list_st *, size_t);
void bytecode_list_append(struct bytecode_list_st *, struct bytecode_st *);
void bytecode_list_add_new(struct bytecode_list_st *);
struct bytecode_st *bytecode_list_last(struct bytecode_list_st *);
struct bytecode_st *bytecode_list_pop(struct bytecode_list_st *res);


void print_bytecode_list(const struct bytecode_list_st *, int);

#endif //BYTECODE_LIST_H
