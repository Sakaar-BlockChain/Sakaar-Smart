#ifndef TK_SPECIAL_H
#define TK_SPECIAL_H

#include "sc_structs.h"


// Special Chars
#define Special_None        0x00

// Special Chars OP
#define Special_MOD         0x11
#define Special_AND         0x12
#define Special_MUL         0x13
#define Special_ADD         0x14
#define Special_SUB         0x15
#define Special_DIV         0x16
#define Special_XOR         0x17
#define Special_OR          0x18
#define Special_LSHIFT      0x19
#define Special_RSHIFT      0x1a

// Special Chars OP EQ
#define Special_EQ_MOD      0x21
#define Special_EQ_AND      0x22
#define Special_EQ_MUL      0x23
#define Special_EQ_ADD      0x24
#define Special_EQ_SUB      0x25
#define Special_EQ_DIV      0x26
#define Special_EQ_XOR      0x27
#define Special_EQ_OR       0x28
#define Special_EQ_LSHIFT   0x29
#define Special_EQ_RSHIFT   0x2a

// Special Chars Brackets
#define Special_LSB         0x31
#define Special_RSB         0x32
#define Special_LSQB        0x33
#define Special_RSQB        0x34
#define Special_LCB         0x35
#define Special_RCB         0x36

// Special Chars Compare
#define Special_LESS        0x41
#define Special_GREATER     0x42
#define Special_EQ_LESS     0x43
#define Special_EQ_GREATER  0x44
#define Special_EQ_NOT      0x45
#define Special_EQ_EQ       0x46

// Special Chars Bool OP
#define Special_AND_AND     0x51
#define Special_OR_OR       0x52

// Special Chars Comments
#define Special_COM_LINE    0x61
#define Special_COM_STR     0x62
#define Special_COM_END     0x63


#define Special_COMMA       0x01
#define Special_DOT         0x02
#define Special_COLON       0x03
#define Special_SEMI        0x04
#define Special_EQ          0x05
#define Special_NOT         0x06


void tokenize_special(struct tk_token *, struct sc_parser *);

#endif //TK_SPECIAL_H
