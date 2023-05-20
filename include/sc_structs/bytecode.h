#ifndef BYTECODE_H
#define BYTECODE_H

#include "struct.h"

#define BC_Basics           0x00
#define BC_Init             0x01
#define BC_Load             0x02
#define BC_LoadConst        0x03
#define BC_Attrib           0x04
#define BC_Attrib_Del       0x05
#define BC_Subscript        0x06
#define BC_Call             0x07
#define BC_FuncEnd          0x08
#define BC_ClassEnd         0x09
#define BC_Pop              0x0a

#define BC_Convert          0x10
#define BC_Convert_Bool     0x11
#define BC_Convert_Int      0x12
#define BC_Convert_Float    0x13
#define BC_Convert_Str      0x14

#define BC_Operations       0x20
#define BC_Compare          0x21
#define BC_Arithmetic       0x22
#define BC_ArithmeticSet    0x23
#define BC_Negate           0x24
#define BC_NegateBool       0x25
#define BC_Set              0x26

#define BC_Jump             0x30
#define BC_IfTrueOrPop      0x31
#define BC_IfFalseOrPop     0x32
#define BC_IfFalse_Jump     0x33

#define BC_Make             0x40
#define BC_MakeFunc         0x41
#define BC_MakeClass        0x42
#define BC_MakeList         0x43

#define BC_Interrupts       0x50
#define BC_Break            0x51
#define BC_Throw            0x52
#define BC_Return           0x53
#define BC_Continue         0x54

struct bytecode_st {
    char *command;
    void **data;

    size_t max_size, size;
};

struct bytecode_st *bytecode_new();
void bytecode_free(struct bytecode_st *);

void bytecode_resize(struct bytecode_st *, size_t);
void bytecode_append(struct bytecode_st *, char , void *);

void print_code(char command, void *data);
void print_bytecode(const struct bytecode_st *, int);

#endif //BYTECODE_H
