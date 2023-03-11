#ifndef TK_TOKEN_H
#define TK_TOKEN_H

#include "struct.h"

#define TokenType_None         0x00
#define TokenType_KeyWords     0x01
#define TokenType_Identifier   0x02
#define TokenType_String       0x03
#define TokenType_Int          0x04
#define TokenType_Special      0x05

#define IntType_bin             0x01
#define IntType_oct             0x02
#define IntType_dec             0x03
#define IntType_hex             0x04
#define IntType_float           0x10


struct tk_token{
    short type;
    short subtype;

    size_t size, max_size;
    char *data;

    size_t pos, line_num, line_pos;
};


struct tk_token *tk_token_new();
void tk_token_set(struct tk_token *, const struct tk_token *);
void tk_token_clear(struct tk_token *);
void tk_token_free(struct tk_token *);

void tk_token_resize(struct tk_token *, size_t);
void tk_token_set_pos(struct tk_token *, struct sc_parser *);



#endif //TK_TOKEN_H
