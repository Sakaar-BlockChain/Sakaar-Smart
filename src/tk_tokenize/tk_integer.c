#include "tk_tokenize.h"

#define CharInt_bin(c) ((c) >= '0' && (c) <= '1')
#define CharInt_oct(c) ((c) >= '0' && (c) <= '7')
#define CharInt_dec(c) ((c) >= '0' && (c) <= '9')
#define CharInt_hex(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))

#define ErrInt {                                                                        \
sc_error_set_msg(parser->error, ErrorType_Tokenizer,"Error while parsing integer");     \
sc_error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos);  \
return;}
#define GetChar {pos++;if (pos == parser->data_size) ErrInt c = parser->data_str[pos];}

void tokenize_integer(struct token_st *token, struct parser_st *parser) {
    size_t pos = parser->data_pos;
    if (CharInt_dec(parser->data_str[pos])) {
        char c = parser->data_str[pos];
        if (c == '0') {
            GetChar
            if (c == 'b' || c == 'B') {
                GetChar
                while(c == '_') GetChar
                size_t zero_end = pos;
                while(CharInt_bin(c)) GetChar
                if(IdentifierStart(c)) ErrInt

                token->type = TokenType_Int;
                token->sub_type = IntType_bin;

                string_set_str(&token->data,&parser->data_str[zero_end], pos - zero_end);
                token_set_pos(token, parser);
                parser->data_pos = pos;
            } else if (c == 'o' || c == 'O') {
                GetChar
                while(c == '_') GetChar
                size_t zero_end = pos;
                while(CharInt_oct(c)) GetChar
                if(IdentifierStart(c)) ErrInt

                token->type = TokenType_Int;
                token->sub_type = IntType_oct;

                string_set_str(&token->data,&parser->data_str[zero_end], pos - zero_end);
                token_set_pos(token, parser);
                parser->data_pos = pos;
            } else if (c == 'x' || c == 'X') {
                GetChar
                while(c == '_') GetChar
                size_t zero_end = pos;
                while(CharInt_hex(c)) GetChar
                if(IdentifierStart(c)) ErrInt

                token->type = TokenType_Int;
                token->sub_type = IntType_hex;

                string_set_str(&token->data,&parser->data_str[zero_end], pos - zero_end);
                token_set_pos(token, parser);
                parser->data_pos = pos;
            } else {
                while(c == '0') GetChar
                size_t zero_end = pos;
                while(CharInt_dec(c)) GetChar

                if (c == '.') {
                    GetChar
                    while(CharInt_dec(c)) GetChar
                    token->sub_type = IntType_float;
                } else {
                    token->sub_type = IntType_dec;
                }
                if(IdentifierStart(c)) ErrInt

                token->type = TokenType_Int;

                string_set_str(&token->data,&parser->data_str[zero_end], pos - zero_end);
                token_set_pos(token, parser);
                parser->data_pos = pos;
            }
        } else {
            while(CharInt_dec(c)) GetChar
            if (c == '.') {
                GetChar
                while(CharInt_dec(c)) GetChar
                token->sub_type = IntType_float;
            } else {
                token->sub_type = IntType_dec;
            }
            if(IdentifierStart(c)) ErrInt

            token->type = TokenType_Int;

            string_set_str(&token->data,&parser->data_str[parser->data_pos], pos - parser->data_pos);
            token_set_pos(token, parser);
            parser->data_pos = pos;
        }
    }
}