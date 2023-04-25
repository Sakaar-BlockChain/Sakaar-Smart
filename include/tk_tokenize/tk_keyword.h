#ifndef TK_KEYWORD_H
#define TK_KEYWORD_H

#include "sc_structs.h"


// Not Defined Keyword
#define KeyWord_None            0x00

// Keywords length 2
#define KeyWord_AS              0x20
#define KeyWord_DO              0x21
#define KeyWord_IF              0x22
#define KeyWord_IN              0x23

// Keywords length 3
#define KeyWord_TRY             0x30
#define KeyWord_FOR             0x31
#define KeyWord_VAR             0x32

// Keywords length 4
#define KeyWord_CASE            0x40
#define KeyWord_ELSE            0x41
#define KeyWord_NONE            0x42
#define KeyWord_TRUE            0x43
#define KeyWord_FROM            0x44
#define KeyWord_FUNC            0x45
#define KeyWord_WITH            0x46

// Keywords length 5
#define KeyWord_BREAK           0x50
#define KeyWord_CLASS           0x51
#define KeyWord_WHILE           0x52
#define KeyWord_FALSE           0x53

// Keywords length 6
#define KeyWord_PUBLIC          0x60
#define KeyWord_SWITCH          0x61
#define KeyWord_STATIC          0x62
#define KeyWord_RETURN          0x63
#define KeyWord_IMPORT          0x64

// Keywords length 7
#define KeyWord_PRIVATE         0x70

// Keywords length 8
#define KeyWord_CONTINUE        0x80

void tokenize_keyword(struct token_st *, struct parser_st *);

#endif //TK_KEYWORD_H
