#include "tk_tokenize.h"


short Special_OneChar(char c1) {
    switch (c1) {
        case '%':
            return Special_MOD;
        case '&':
            return Special_AND;
        case '(':
            return Special_LSB;
        case ')':
            return Special_RSB;
        case '*':
            return Special_MUL;
        case '+':
            return Special_ADD;
        case ',':
            return Special_COMMA;
        case '-':
            return Special_SUB;
        case '.':
            return Special_DOT;
        case '/':
            return Special_DIV;
        case ':':
            return Special_COLON;
        case ';':
            return Special_SEMI;
        case '<':
            return Special_LESS;
        case '=':
            return Special_EQ;
        case '>':
            return Special_GREATER;
        case '!':
            return Special_NOT;
        case '[':
            return Special_LSQB;
        case ']':
            return Special_RSQB;
        case '^':
            return Special_XOR;
        case '{':
            return Special_LCB;
        case '|':
            return Special_OR;
        case '}':
            return Special_RCB;
        default:
            break;
    }
    return Special_None;
}
short Special_TwoChar(char c1, char c2) {
    switch (c1) {
        case '%':
            if (c2 == '=') return Special_EQ_MOD;
            break;
        case '&':
            if (c2 == '&') return Special_AND_AND;
            if (c2 == '=') return Special_EQ_AND;
            break;
        case '*':
            if (c2 == '=') return Special_EQ_MUL;
            if (c2 == '/') return Special_COM_END;
            break;
        case '+':
            if (c2 == '=') return Special_EQ_ADD;
            break;
        case '-':
            if (c2 == '=') return Special_EQ_SUB;
            break;
        case '/':
            if (c2 == '=') return Special_EQ_DIV;
            if (c2 == '/') return Special_COM_LINE;
            if (c2 == '*') return Special_COM_STR;
            break;
        case '<':
            if (c2 == '<') return Special_LSHIFT;
            if (c2 == '=') return Special_EQ_LESS;
            break;
        case '=':
            if (c2 == '=') return Special_EQ_EQ;
            break;
        case '>':
            if (c2 == '>') return Special_RSHIFT;
            if (c2 == '=') return Special_EQ_GREATER;
            break;
        case '!':
            if (c2 == '=') return Special_EQ_NOT;
            break;
        case '^':
            if (c2 == '=') return Special_EQ_XOR;
            break;
        case '|':
            if (c2 == '|') return Special_OR_OR;
            if (c2 == '=') return Special_EQ_OR;
            break;
        default:
            break;
    }
    return Special_None;
}
short Special_ThreeChar(char c1, char c2, char c3) {
    switch (c1) {
        case '<':
            if (c2 == '<' && c3 == '=') return Special_EQ_LSHIFT;
            break;
        case '>':
            if (c2 == '>' && c3 == '=') return Special_EQ_RSHIFT;
            break;
        default:
            break;
    }
    return Special_None;
}


void tokenize_special(struct tk_token *token, struct sc_parser *parser) {
    short result = Special_OneChar(parser->data[parser->position]);
    if (result != Special_None) {
        token->type = TokenType_Special;
        tk_token_set_pos(token, parser->line_pos, parser->position);

        token->subtype = result;
        parser->position++;

        result = Special_TwoChar(parser->data[parser->position - 1], parser->data[parser->position]);
        if (result != Special_None) {
            token->subtype = result;
            parser->position++;

            result = Special_ThreeChar(parser->data[parser->position - 2], parser->data[parser->position - 1],
                                           parser->data[parser->position]);
            if (result != Special_None) {
                token->subtype = result;
                parser->position++;
            }
        }
    }
}