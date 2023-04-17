#include "tk_tokenize.h"

int tokenize_keyword_cmp(struct parser_st *parser, const char *data, size_t size) {
    if(parser->data_size - parser->data_pos < size) return 0;
    if(memcmp(&parser->data_str[parser->data_pos], data, size) != 0) return 0;
    if (parser->data_size - parser->data_pos >= size + 1 && IdentifierChar(parser->data_str[parser->data_pos + size]))
        return 0;
    return 1;
}


void tokenize_keyword(struct token_st *token, struct parser_st *parser) {
    // Keywords length 2
    if (tokenize_keyword_cmp(parser, "do", 2)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_DO;
        token_set_pos(token, parser);
        parser->data_pos += 2;
        return;
    }
    if (tokenize_keyword_cmp(parser, "if", 2)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_IF;
        token_set_pos(token, parser);
        parser->data_pos += 2;
        return;
    }
    if (tokenize_keyword_cmp(parser, "in", 2)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_IN;
        token_set_pos(token, parser);
        parser->data_pos += 2;
        return;
    }
    // Keywords length 3
    if (tokenize_keyword_cmp(parser, "for", 3)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_FOR;
        token_set_pos(token, parser);
        parser->data_pos += 3;
        return;
    }
    if (tokenize_keyword_cmp(parser, "var", 3)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_VAR;
        token_set_pos(token, parser);
        parser->data_pos += 3;
        return;
    }
    // Keywords length 4
    if (tokenize_keyword_cmp(parser, "case", 4)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_CASE;
        token_set_pos(token, parser);
        parser->data_pos += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "else", 4)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_ELSE;
        token_set_pos(token, parser);
        parser->data_pos += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "None", 4)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_NONE;
        token_set_pos(token, parser);
        parser->data_pos += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "True", 4)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_TRUE;
        token_set_pos(token, parser);
        parser->data_pos += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "func", 4)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_FUNC;
        token_set_pos(token, parser);
        parser->data_pos += 4;
        return;
    }
    // Keywords length 5
    if (tokenize_keyword_cmp(parser, "break", 5)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_BREAK;
        token_set_pos(token, parser);
        parser->data_pos += 5;
        return;
    }
    if (tokenize_keyword_cmp(parser, "class", 5)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_CLASS;
        token_set_pos(token, parser);
        parser->data_pos += 5;
        return;
    }
    if (tokenize_keyword_cmp(parser, "while", 5)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_WHILE;
        token_set_pos(token, parser);
        parser->data_pos += 5;
        return;
    }
    if (tokenize_keyword_cmp(parser, "False", 5)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_FALSE;
        token_set_pos(token, parser);
        parser->data_pos += 5;
        return;
    }
    // Keywords length 6
    if (tokenize_keyword_cmp(parser, "public", 6)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_PUBLIC;
        token_set_pos(token, parser);
        parser->data_pos += 6;
        return;
    }
    if (tokenize_keyword_cmp(parser, "switch", 6)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_SWITCH;
        token_set_pos(token, parser);
        parser->data_pos += 6;
        return;
    }
    if (tokenize_keyword_cmp(parser, "static", 6)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_STATIC;
        token_set_pos(token, parser);
        parser->data_pos += 6;
        return;
    }
    if (tokenize_keyword_cmp(parser, "return", 6)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_RETURN;
        token_set_pos(token, parser);
        parser->data_pos += 6;
        return;
    }
    // Keywords length 7
    if (tokenize_keyword_cmp(parser, "private", 7)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_PRIVATE;
        token_set_pos(token, parser);
        parser->data_pos += 7;
        return;
    }
    // Keywords length 6
    if (tokenize_keyword_cmp(parser, "continue", 8)) {
        token->type = TokenType_KeyWords;
        token->sub_type = KeyWord_CONTINUE;
        token_set_pos(token, parser);
        parser->data_pos += 8;
        return;
    }
}