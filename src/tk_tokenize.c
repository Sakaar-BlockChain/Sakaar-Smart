#include "tk_tokenize.h"

void tokenize_parse(struct tk_token *token, struct sc_parser *parser){
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

void tokenize(struct sc_parser *parser) {
    parser->position = parser->current_line = 0;
    struct tk_token *token = tk_token_new();
    int comment_type = 0;
    while (parser->position < parser->str_size) {
        if (SpaceChar(parser->data[parser->position])) {
            if (parser->data[parser->position] == '\n') {
                if (comment_type == 1) comment_type = 0;
                parser->current_line++;
            }
            parser->position++;
            continue;
        }
        if (comment_type != 0) {
            if (comment_type == 2 && parser->data[parser->position] == '*' && parser->data[parser->position + 1] == '/') {
                parser->position++;
                comment_type = 0;
            }
            parser->position++;
            continue;
        }
        tokenize_parse(token, parser);
        if (token->type == TokenType_None) {
            if (string_is_null(parser->error_msg)) string_set_str(parser->error_msg, "Unrecognized token", 18);
            goto bad_end;
        }
        if (token->type == TokenType_Special) {
            switch (token->subtype) {
                case Special_COM_LINE:
                    comment_type = 1;
                    break;
                case Special_COM_STR:
                    comment_type = 2;
                    break;
                case Special_LSB:
                    parser->scope_buf[parser->scope_pos++] = Special_LSB;
                    if (parser->scope_pos > MaxBracketNesting) {
                        string_set_str(parser->error_msg, "Scope length more then max scopes nesting", 41);
                        goto bad_end;
                    }
                    break;
                case Special_LSQB:
                    parser->scope_buf[parser->scope_pos++] = Special_LSQB;
                    if (parser->scope_pos > MaxBracketNesting) {
                        string_set_str(parser->error_msg, "Scope length more then max scopes nesting", 41);
                        goto bad_end;
                    }
                    break;
                case Special_LCB:
                    parser->scope_buf[parser->scope_pos++] = Special_LCB;
                    if (parser->scope_pos > MaxBracketNesting) {
                        string_set_str(parser->error_msg, "Scope length more then max scopes nesting", 41);
                        goto bad_end;
                    }
                    break;
                case Special_RSB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LSB) {
                        if (parser->scope_buf[parser->scope_pos] == Special_LSQB) {
                            string_set_str(parser->error_msg, "Scope closed incorrectly. Must be ']' using ')'", 47);
                        } else if (parser->scope_buf[parser->scope_pos] == Special_LCB) {
                            string_set_str(parser->error_msg, "Scope closed incorrectly. Must be '}' using ')'", 47);
                        }
                        goto bad_end;
                    }
                    break;
                case Special_RSQB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LSQB) {
                        if (parser->scope_buf[parser->scope_pos] == Special_LSB) {
                            string_set_str(parser->error_msg, "Scope closed incorrectly. Must be ')' using ']'", 47);
                        } else if (parser->scope_buf[parser->scope_pos] == Special_LCB) {
                            string_set_str(parser->error_msg, "Scope closed incorrectly. Must be '}' using ']'", 47);
                        }
                        goto bad_end;
                    }
                    break;
                case Special_RCB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LCB) {
                        if (parser->scope_buf[parser->scope_pos] == Special_LSQB) {
                            string_set_str(parser->error_msg, "Scope closed incorrectly. Must be ']' using '}'", 47);
                        } else if (parser->scope_buf[parser->scope_pos] == Special_LSB) {
                            string_set_str(parser->error_msg, "Scope closed incorrectly. Must be ')' using '}'", 47);
                        }
                        goto bad_end;
                    }
                    break;
                default:
                    break;
            }
        }
        if (comment_type == 0) {
            list_add_new(parser->list, TK_TOKEN_TYPE);
            tk_token_set(parser->list->data[parser->list->size - 1]->data, token);
        }
        tk_token_clear(token);
    }

    if (parser->scope_pos != 0) {
        string_set_str(parser->error_msg, "Scope is not closed...", 22);
        goto bad_end;
    }
    tk_token_free(token);
    return;

    bad_end:
    tk_token_free(token);
    list_clear(parser->list);
}
