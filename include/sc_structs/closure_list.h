#ifndef CLOSURE_LIST_H
#define CLOSURE_LIST_H

#include "struct.h"

struct closure_list_st{
    struct closure_st **closures;
    size_t size;
    size_t max_size;
    int type;
};

void closure_list_set(struct closure_list_st *, const struct closure_list_st *);
void closure_list_clear(struct closure_list_st *);

void closure_list_data_init(struct closure_list_st *);
void closure_list_data_free(struct closure_list_st *);

void closure_list_resize(struct closure_list_st *, size_t);
void closure_list_append(struct closure_list_st *, struct closure_st *);
void closure_list_add_new(struct closure_list_st *);
struct closure_st *closure_list_last(struct closure_list_st *);

#endif //CLOSURE_LIST_H
