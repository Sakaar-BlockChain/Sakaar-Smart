#ifndef TK_KEYWORD_H
#define TK_KEYWORD_H

#include "sc_structs.h"


// Not Defined Keyword
#define KeyWord_None            0x00

// Keywords length 2
#define KeyWord_DO              0x01
#define KeyWord_IF              0x02
#define KeyWord_IN              0x03

// Keywords length 3
#define KeyWord_FOR             0x04
#define KeyWord_NULL            0x05

// Keywords length 4
#define KeyWord_CASE            0x06
#define KeyWord_ELSE            0x07
#define KeyWord_TRUE            0x08
#define KeyWord_FUNC            0x09

// Keywords length 5
#define KeyWord_BREAK           0x0a
#define KeyWord_CLASS           0x0b
#define KeyWord_WHILE           0x0c
#define KeyWord_FALSE           0x0d

// Keywords length 6
#define KeyWord_SWITCH          0x0e
#define KeyWord_STATIC          0x0f
#define KeyWord_RETURN          0x10

// Keywords length 8
#define KeyWord_CONTINUE        0x11

void tokenize_keyword(struct tk_token *, struct tk_parser *);

#endif //TK_KEYWORD_H
