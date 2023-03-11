#include "tk_tokenize.h"

int tokenize_keyword_cmp(struct sc_parser *parser, const char *data, size_t size) {
    if(parser->size - parser->pos < size) return 0;
    if(memcmp(&parser->data[parser->pos], data, size) != 0) return 0;
    if (parser->size - parser->pos >= size + 1 && IdentifierChar(parser->data[parser->pos + size]))
        return 0;
    return 1;
}


void tokenize_keyword(struct tk_token *token, struct sc_parser *parser) {
    // Keywords length 2
    if (tokenize_keyword_cmp(parser, "do", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_DO;
        tk_token_set_pos(token, parser);
        parser->pos += 2;
        return;
    }
    if (tokenize_keyword_cmp(parser, "if", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_IF;
        tk_token_set_pos(token, parser);
        parser->pos += 2;
        return;
    }
    if (tokenize_keyword_cmp(parser, "in", 2)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_IN;
        tk_token_set_pos(token, parser);
        parser->pos += 2;
        return;
    }
    // Keywords length 3
    if (tokenize_keyword_cmp(parser, "for", 3)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FOR;
        tk_token_set_pos(token, parser);
        parser->pos += 3;
        return;
    }
    // Keywords length 4
    if (tokenize_keyword_cmp(parser, "case", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_CASE;
        tk_token_set_pos(token, parser);
        parser->pos += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "else", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_ELSE;
        tk_token_set_pos(token, parser);
        parser->pos += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "None", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_NONE;
        tk_token_set_pos(token, parser);
        parser->pos += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "True", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_TRUE;
        tk_token_set_pos(token, parser);
        parser->pos += 4;
        return;
    }
    if (tokenize_keyword_cmp(parser, "func", 4)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FUNC;
        tk_token_set_pos(token, parser);
        parser->pos += 4;
        return;
    }
    // Keywords length 5
    if (tokenize_keyword_cmp(parser, "break", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_BREAK;
        tk_token_set_pos(token, parser);
        parser->pos += 5;
        return;
    }
    if (tokenize_keyword_cmp(parser, "class", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_CLASS;
        tk_token_set_pos(token, parser);
        parser->pos += 5;
        return;
    }
    if (tokenize_keyword_cmp(parser, "while", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_WHILE;
        tk_token_set_pos(token, parser);
        parser->pos += 5;
        return;
    }
    if (tokenize_keyword_cmp(parser, "False", 5)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_FALSE;
        tk_token_set_pos(token, parser);
        parser->pos += 5;
        return;
    }
    // Keywords length 6
    if (tokenize_keyword_cmp(parser, "public", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_PUBLIC;
        tk_token_set_pos(token, parser);
        parser->pos += 6;
        return;
    }
    if (tokenize_keyword_cmp(parser, "switch", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_SWITCH;
        tk_token_set_pos(token, parser);
        parser->pos += 6;
        return;
    }
    if (tokenize_keyword_cmp(parser, "static", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_STATIC;
        tk_token_set_pos(token, parser);
        parser->pos += 6;
        return;
    }
    if (tokenize_keyword_cmp(parser, "return", 6)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_RETURN;
        tk_token_set_pos(token, parser);
        parser->pos += 6;
        return;
    }
    // Keywords length 7
    if (tokenize_keyword_cmp(parser, "private", 7)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_PRIVATE;
        tk_token_set_pos(token, parser);
        parser->pos += 7;
        return;
    }
    // Keywords length 6
    if (tokenize_keyword_cmp(parser, "continue", 8)) {
        token->type = TokenType_KeyWords;
        token->subtype = KeyWord_CONTINUE;
        tk_token_set_pos(token, parser);
        parser->pos += 8;
        return;
    }
}