#ifndef BLOCK_H
#define BLOCK_H

#include "struct.h"
#include "node_list.h"

struct block_st{
    short type;
    short sub_type;

    struct node_list_st nodes;
    struct token_st *token;
};

#endif //BLOCK_H
