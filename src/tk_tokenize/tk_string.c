#include "tk_tokenize.h"

void tokenize_string(struct token_st *token, struct parser_st *parser){
    if(parser->data_str[parser->data_pos] != '"' && parser->data_str[parser->data_pos] != '\'') return;

    size_t pos = parser->data_pos + 1;
    if (parser->data_str[parser->data_pos] == '"') {
        for (;pos < parser->data_size; pos++) {
            if ((pos == 0 || parser->data_str[pos - 1] != '\\') && parser->data_str[pos] == '"') break;
        }
        if (parser->data_size == pos) {
            error_set_msg(parser->error, ErrorType_Tokenizer, "String started but did not end use for this \" ");
            error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos);
            return;
        }
    } else {
        for (;pos < parser->data_size; pos++) {
            if ((pos == 0 || parser->data_str[pos - 1] != '\\') && parser->data_str[pos] == '\'') break;
        }
        if (parser->data_size == pos) {
            error_set_msg(parser->error, ErrorType_Tokenizer, "String started but did not end use for this ' ");
            error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos);
            return;
        }
    }

    token->type = TokenType_String;

    string_set_str(&token->data, &parser->data_str[parser->data_pos + 1], pos - parser->data_pos - 1);
    token_set_pos(token, parser);
    parser->data_pos = pos + 1;
}