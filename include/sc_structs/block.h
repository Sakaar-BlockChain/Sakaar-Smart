#ifndef BLOCK_H
#define BLOCK_H

#include "struct.h"
#include "block_list.h"

#define BlockType_None  0x00

#define BlockType_None          0x00
#define BlockType_ByteCode      0x02

struct block_expr_st{
    short type;
    short sub_type;

    struct bytecode_st *code;
    struct node_st *node;
    
    struct block_list_st blocks;
};

struct block_expr_st *block_expr_new();
void block_expr_set(struct block_expr_st *, const struct block_expr_st *);
void block_expr_clear(struct block_expr_st *);
void block_expr_free(struct block_expr_st *);

void block_expr_data_init(struct block_expr_st *);
void block_expr_data_free(struct block_expr_st *);


void print_block(const struct block_expr_st *, int);

#endif //BLOCK_H
