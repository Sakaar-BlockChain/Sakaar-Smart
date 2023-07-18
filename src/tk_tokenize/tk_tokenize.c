#include "tk_tokenize.h"

void tokenize_parse(struct token_st *token, struct parser_st *parser) {
    tokenize_special(token, parser);
    if (token->type != TokenType_None) return;
    tokenize_string(token, parser);
    if (token->type != TokenType_None) return;
    tokenize_integer(token, parser);
    if (token->type != TokenType_None) return;
    tokenize_keyword(token, parser);
    if (token->type != TokenType_None) return;
    tokenize_identifier(token, parser);
}

void tokenize(struct parser_st *parser) {
    parser->data_pos = parser->line_num = parser->line_pos = 0;
    struct token_st *token = token_new();
    int comment_type = 0;

    while (parser->data_pos < parser->data_size) {
        if (SpaceChar(parser->data_str[parser->data_pos])) {
            if (parser->data_str[parser->data_pos] == '\n') {
                if (comment_type == 1) comment_type = 0;
                parser->line_num++;
                parser->line_pos = parser->data_pos + 1;
            }
            parser->data_pos++;
            continue;
        }
        if (comment_type != 0) {
            if (comment_type == 2 && parser->data_str[parser->data_pos] == '*' && parser->data_str[parser->data_pos + 1] == '/') {
                parser->data_pos++;
                comment_type = 0;
            }
            parser->data_pos++;
            continue;
        }
        tokenize_parse(token, parser);
        if (token->type == TokenType_None) {
            if (!parser->error->present) {
                error_set_msg(parser->error, ErrorType_Tokenizer, "Unrecognized token");
                error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos - 1);
            }
            goto bad_end;
        }
        if (token->type == TokenType_Special) {
            switch (token->sub_type) {
                case Special_COM_LINE:
                    comment_type = 1;
                    break;
                case Special_COM_STR:
                    comment_type = 2;
                    break;
                case Special_LSB:
                    parser->scope_buf[parser->scope_pos++] = Special_LSB;
                    if (parser->scope_pos > MaxBracketNesting) {
                        error_set_msg(parser->error, ErrorType_Tokenizer, "Scope length more then max scopes nesting");
                        error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos - 1);
                        goto bad_end;
                    }
                    break;
                case Special_LSQB:
                    parser->scope_buf[parser->scope_pos++] = Special_LSQB;
                    if (parser->scope_pos > MaxBracketNesting) {
                        error_set_msg(parser->error, ErrorType_Tokenizer, "Scope length more then max scopes nesting");
                        error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos - 1);
                        goto bad_end;
                    }
                    break;
                case Special_LCB:
                    parser->scope_buf[parser->scope_pos++] = Special_LCB;
                    if (parser->scope_pos > MaxBracketNesting) {
                        error_set_msg(parser->error, ErrorType_Tokenizer, "Scope length more then max scopes nesting");
                        error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos - 1);
                        goto bad_end;
                    }
                    break;
                case Special_RSB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LSB) {
                        if (parser->scope_buf[parser->scope_pos] == Special_LSQB) {
                            error_set_msg(parser->error, ErrorType_Tokenizer, "Scope closed incorrectly. Must be ']' using ')'");
                        } else if (parser->scope_buf[parser->scope_pos] == Special_LCB) {
                            error_set_msg(parser->error, ErrorType_Tokenizer, "Scope closed incorrectly. Must be '}' using ')'");
                        }
                        error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos - 1);
                        goto bad_end;
                    }
                    break;
                case Special_RSQB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LSQB) {
                        if (parser->scope_buf[parser->scope_pos] == Special_LSB) {
                            error_set_msg(parser->error, ErrorType_Tokenizer, "Scope closed incorrectly. Must be ')' using ']'");
                        } else if (parser->scope_buf[parser->scope_pos] == Special_LCB) {
                            error_set_msg(parser->error, ErrorType_Tokenizer, "Scope closed incorrectly. Must be '}' using ']'");
                        }
                        error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos - 1);
                        goto bad_end;
                    }
                    break;
                case Special_RCB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LCB) {
                        if (parser->scope_buf[parser->scope_pos] == Special_LSQB) {
                            error_set_msg(parser->error, ErrorType_Tokenizer, "Scope closed incorrectly. Must be ']' using '}'");
                        } else if (parser->scope_buf[parser->scope_pos] == Special_LSB) {
                            error_set_msg(parser->error, ErrorType_Tokenizer, "Scope closed incorrectly. Must be ')' using '}'");
                        }
                        error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos - 1);
                        goto bad_end;
                    }
                    break;
                default:
                    break;
            }
        }
        if (comment_type == 0) {
            token_list_add_new(&parser->tokens);
            token_set(token_list_last(&parser->tokens), token);
        }
        token_clear(token);
    }

    if (parser->scope_pos != 0) {
        error_set_msg(parser->error, ErrorType_Tokenizer, "Scope is not closed...");
        error_set_pos(parser->error, parser->line_num, parser->line_pos, parser->data_pos - 1);
        goto bad_end;
    }
    token_free(token);
    return;

    bad_end:
    token_free(token);
    token_list_clear(&parser->tokens);
}
