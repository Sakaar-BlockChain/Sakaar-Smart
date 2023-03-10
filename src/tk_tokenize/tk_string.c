#include "tk_tokenize.h"

void tokenize_string(struct tk_token *token, struct sc_parser *parser){
    if(parser->data[parser->position] != '"' && parser->data[parser->position] != '\'') return;

    size_t pos = parser->position + 1;
    if (parser->data[parser->position] == '"') {
        for (;pos < parser->str_size; pos++) {
            if ((pos == 0 || parser->data[pos - 1] != '\\') && parser->data[pos] == '"') break;
        }
        if (parser->str_size == pos) {
            string_set_str(parser->error_msg, "String started but did not end use for this \" ", 47);
            return;
        }
    } else {
        for (;pos < parser->str_size; pos++) {
            if ((pos == 0 || parser->data[pos - 1] != '\\') && parser->data[pos] == '\'') break;
        }
        if (parser->str_size == pos) {
            string_set_str(parser->error_msg, "String started but did not end use for this ' ", 47);
            return;
        }
    }

    token->type = TokenType_String;
    tk_token_resize(token, pos - parser->position - 1);
    memcpy(token->data, &parser->data[parser->position + 1], token->size);
    tk_token_set_pos(token, parser->line_pos, parser->position);
    parser->position = pos + 1;
}