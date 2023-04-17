#ifndef NODE_LIST_H
#define NODE_LIST_H

#include "struct.h"

struct node_list_st{
    struct node_st **nodes;
    size_t size;
    size_t max_size;
    int type;
};

void node_list_set(struct node_list_st *, const struct node_list_st *);
void node_list_clear(struct node_list_st *);

void node_list_data_init(struct node_list_st *);
void node_list_data_free(struct node_list_st *);

void node_list_resize(struct node_list_st *, size_t);
void node_list_append(struct node_list_st *, struct node_st *);
void node_list_add_new(struct node_list_st *);
struct node_st *node_list_last(struct node_list_st *);

#endif //NODE_LIST_H
