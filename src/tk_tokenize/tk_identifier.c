#include "tk_tokenize.h"

void tokenize_identifier(struct tk_token *token, struct tk_parser *parser){
    if (IdentifierStart(parser->data[parser->position])) {
        size_t pos = parser->position;
        for (; pos < parser->str_size; pos++) {
            if (!IdentifierChar(parser->data[pos])) break;
        }
        token->type = TokenType_Identifier;
        tk_token_resize(token, pos - parser->position);
        memcpy(token->data, &parser->data[parser->position], token->size);
        parser->position = pos;
    }
}