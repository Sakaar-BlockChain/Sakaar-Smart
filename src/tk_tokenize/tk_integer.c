#include "tk_tokenize.h"

#define CharInt_bin(c) ((c) >= '0' && (c) <= '1')
#define CharInt_oct(c) ((c) >= '0' && (c) <= '7')
#define CharInt_dec(c) ((c) >= '0' && (c) <= '9')
#define CharInt_hex(c) (((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))

#define ErrInt {string_set_str(parser->error_msg, "Error while parsing integer", 27); return;}
#define GetChar {pos++;if (pos == parser->size) ErrInt c = parser->data[pos];}

void tokenize_integer(struct tk_token *token, struct sc_parser *parser) {
    size_t pos = parser->pos;
    if (CharInt_dec(parser->data[pos])) {
        char c = parser->data[pos];
        if (c == '0') {
            GetChar
            if (c == 'b' || c == 'B') {
                GetChar
                while(c == '_') GetChar
                size_t zero_end = pos;
                while(CharInt_bin(c)) GetChar
                if(IdentifierStart(c)) ErrInt

                token->type = TokenType_Int;
                token->subtype = IntType_bin;
                tk_token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                tk_token_set_pos(token, parser);
                parser->pos = pos;
            } else if (c == 'o' || c == 'O') {
                GetChar
                while(c == '_') GetChar
                size_t zero_end = pos;
                while(CharInt_oct(c)) GetChar
                if(IdentifierStart(c)) ErrInt

                token->type = TokenType_Int;
                token->subtype = IntType_oct;
                tk_token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                tk_token_set_pos(token, parser);
                parser->pos = pos;
            } else if (c == 'x' || c == 'X') {
                GetChar
                while(c == '_') GetChar
                size_t zero_end = pos;
                while(CharInt_hex(c)) GetChar
                if(IdentifierStart(c)) ErrInt

                token->type = TokenType_Int;
                token->subtype = IntType_hex;
                tk_token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                tk_token_set_pos(token, parser);
                parser->pos = pos;
            } else {
                while(c == '0') GetChar
                size_t zero_end = pos;
                while(CharInt_dec(c)) GetChar

                if (c == '.') {
                    GetChar
                    while(CharInt_dec(c)) GetChar
                    token->subtype = IntType_float;
                } else {
                    token->subtype = IntType_dec;
                }
                if(IdentifierStart(c)) ErrInt

                token->type = TokenType_Int;
                tk_token_resize(token, pos - zero_end);
                memcpy(token->data, &parser->data[zero_end], token->size);
                tk_token_set_pos(token, parser);
                parser->pos = pos;
            }
        } else {
            while(CharInt_dec(c)) GetChar
            if (c == '.') {
                GetChar
                while(CharInt_dec(c)) GetChar
                token->subtype = IntType_float;
            } else {
                token->subtype = IntType_dec;
            }
            if(IdentifierStart(c)) ErrInt

            token->type = TokenType_Int;
            tk_token_resize(token, pos - parser->pos);
            memcpy(token->data, &parser->data[parser->pos], token->size);
            tk_token_set_pos(token, parser);
            parser->pos = pos;
        }
    }
}