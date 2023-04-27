#include "tk_tokenize.h"

void tokenize_identifier(struct token_st *token, struct parser_st *parser){
    if (IdentifierStart(parser->data_str[parser->data_pos])) {
        size_t pos = parser->data_pos;
        for (; pos < parser->data_size; pos++) {
            if (!IdentifierChar(parser->data_str[pos])) break;
        }
        token->type = TokenType_Identifier;

        string_set_str(&token->data, &parser->data_str[parser->data_pos], pos - parser->data_pos);
        token_set_pos(token, parser);
        parser->data_pos = pos;
    }
}