#ifndef TOKEN_H
#define TOKEN_H

#include "struct.h"
#include "parser.h"

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

struct token_st{
    short type;
    short sub_type;

    struct string_st data;

    size_t line_pos;
    size_t line_num;
    size_t pos;
};

struct token_st *token_new();
void token_set(struct token_st *, const struct token_st *);
void token_clear(struct token_st *);
void token_free(struct token_st *);

void token_data_init(struct token_st *);
void token_data_free(struct token_st *);

void token_set_pos(struct token_st *, struct parser_st *);


#endif //TOKEN_H
