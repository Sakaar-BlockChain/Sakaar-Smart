#ifndef BLOCK_LIST_H
#define BLOCK_LIST_H

#include "struct.h"

struct block_list_st{
    struct block_expr_st **blocks;
    size_t *blocks_pos;
    size_t size;
    size_t max_size;
    int type;
};

void block_list_set(struct block_list_st *, const struct block_list_st *);
void block_list_clear(struct block_list_st *);

void block_list_data_init(struct block_list_st *);
void block_list_data_free(struct block_list_st *);

void block_list_resize(struct block_list_st *, size_t);
void block_list_append(struct block_list_st *, struct block_expr_st *, size_t);
void block_list_add_new(struct block_list_st *);
struct block_expr_st *block_list_last(struct block_list_st *);
size_t block_list_last_pos(struct block_list_st *);

void print_block_list(const struct block_list_st *, int);


#endif //BLOCK_LIST_H
