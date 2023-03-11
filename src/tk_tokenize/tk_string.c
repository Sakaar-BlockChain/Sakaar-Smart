#include "tk_tokenize.h"

void tokenize_string(struct tk_token *token, struct sc_parser *parser){
    if(parser->data[parser->pos] != '"' && parser->data[parser->pos] != '\'') return;

    size_t pos = parser->pos + 1;
    if (parser->data[parser->pos] == '"') {
        for (;pos < parser->size; pos++) {
            if ((pos == 0 || parser->data[pos - 1] != '\\') && parser->data[pos] == '"') break;
        }
        if (parser->size == pos) {
            string_set_str(parser->error_msg, "String started but did not end use for this \" ", 47);
            return;
        }
    } else {
        for (;pos < parser->size; pos++) {
            if ((pos == 0 || parser->data[pos - 1] != '\\') && parser->data[pos] == '\'') break;
        }
        if (parser->size == pos) {
            string_set_str(parser->error_msg, "String started but did not end use for this ' ", 47);
            return;
        }
    }

    token->type = TokenType_String;
    tk_token_resize(token, pos - parser->pos - 1);
    memcpy(token->data, &parser->data[parser->pos + 1], token->size);
    tk_token_set_pos(token, parser);
    parser->pos = pos + 1;
}