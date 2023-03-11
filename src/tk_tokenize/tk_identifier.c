#include "tk_tokenize.h"

void tokenize_identifier(struct tk_token *token, struct sc_parser *parser){
    if (IdentifierStart(parser->data[parser->pos])) {
        size_t pos = parser->pos;
        for (; pos < parser->size; pos++) {
            if (!IdentifierChar(parser->data[pos])) break;
        }
        token->type = TokenType_Identifier;
        tk_token_resize(token, pos - parser->pos);
        memcpy(token->data, &parser->data[parser->pos], token->size);
        tk_token_set_pos(token, parser);
        parser->pos = pos;
    }
}