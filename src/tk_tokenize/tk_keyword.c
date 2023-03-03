#include "tk_tokenize.h"

int tokenize_keyword_cmp(struct tk_parser *parser, const char *data, size_t size) {
    if(parser->str_size - parser->position < size) return 0;
    if(memcmp(&parser->data[parser->position], data, size) != 0) return 0;
    if (parser->str_size - parser->position >= size + 1 && IdentifierChar(parser->data[parser->position + size]))
        return 0;
    return 1;
}


void tokenize_keyword(struct tk_token *token, struct tk_parser *parser) {
    // Keywords length 2
    if (tokenize_keyword_cmp(parser, "do", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_DO;
        parser->position += 2;
        return;
    }
    if (tokenize_keyword_cmp(parser, "if", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_IF;
        parser->position += 2;
        return;
    }
    if (tokenize_keyword_cmp(parser, "in", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_IN;
        parser->position += 2;
        return;
    }
    // Keywords length 3
    if (tokenize_keyword_cmp(parser, "for", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FOR;
        parser->position += 3;
        return;
    }
    // Keywords length 4
    if (tokenize_keyword_cmp(parser, "case", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_CASE;
        parser->position += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "else", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_ELSE;
        parser->position += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "None", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_NONE;
        parser->position += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "True", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_TRUE;
        parser->position += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "func", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FUNC;
        parser->position += 4;
        return;
    }
    // Keywords length 5
    if (tokenize_keyword_cmp(parser, "break", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_BREAK;
        parser->position += 5;
        return;
    }
    if (tokenize_keyword_cmp(parser, "class", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_CLASS;
        parser->position += 5;
        return;
    }
    if (tokenize_keyword_cmp(parser, "while", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_WHILE;
        parser->position += 5;
        return;
    }
    if (tokenize_keyword_cmp(parser, "False", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FALSE;
        parser->position += 5;
        return;
    }
    // Keywords length 6
    if (tokenize_keyword_cmp(parser, "public", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_PUBLIC;
        parser->position += 6;
        return;
    }
    if (tokenize_keyword_cmp(parser, "switch", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_SWITCH;
        parser->position += 6;
        return;
    }
    if (tokenize_keyword_cmp(parser, "static", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_STATIC;
        parser->position += 6;
        return;
    }
    if (tokenize_keyword_cmp(parser, "return", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_RETURN;
        parser->position += 6;
        return;
    }
    // Keywords length 7
    if (tokenize_keyword_cmp(parser, "private", 7)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_PRIVATE;
        parser->position += 7;
        return;
    }
    // Keywords length 6
    if (tokenize_keyword_cmp(parser, "continue", 8)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_CONTINUE;
        parser->position += 8;
        return;
    }
}