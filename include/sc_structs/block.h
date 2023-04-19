#ifndef BLOCK_H
#define BLOCK_H

#include "struct.h"
#include "block_list.h"

#define BlockType_None  0x00

#define BlockType_None          0x00
#define BlockType_Conf          0x01
#define BlockType_OP            0x02
#define BlockType_Done          0x05
//#define

#define Block_Init              0x01
#define Block_Load              0x02
#define Block_LoadConst         0x03
#define Block_OP                0x04
#define Block_Set               0x05
#define Block_POP               0x06
#define Block_IF                0x07
#define Block_Call              0x08

#define Block_Interrupts        0x40
#define Block_Return            0x41
#define Block_Break             0x42
#define Block_Continue          0x43
#define Block_Throw             0x44


#define Block_Convert           0x10
#define Block_Bool              0x11
#define Block_Int               0x12
#define Block_Float             0x13
#define Block_Str               0x14

#define Block_Frame             0x20
#define Block_FrameInit         0x21
#define Block_FrameClose        0x22

#define Block_Construct         0x30
#define Block_List              0x31
#define Block_Func              0x32
#define Block_Class             0x33

struct block_expr_st{
    short type;
    short sub_type;

    size_t count;

    struct node_st *node;

    struct object_st *data;
    struct attrib_st *attrib;
    struct variable_st *variable;
    struct closure_st *closure;
    struct token_st *token;
    
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
