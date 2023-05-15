#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include "sn_syntax.h"

#define expr_cast {                             \
node_list_add_new(&parser->nodes);              \
expr_next = node_list_last(&parser->nodes);     \
node_set(expr_next, expr); node_clear(expr);    \
node_list_append(&expr->nodes, expr_next);      \
}

#define expr_add {                              \
node_list_add_new(&parser->nodes);              \
expr_next = node_list_last(&parser->nodes);     \
node_list_append(&(expr)->nodes, expr_next);    \
}

#define parser_end if (parser->tokens.size <= parser->data_pos)
#define parser_get token = parser->tokens.tokens[parser->data_pos];

#define analyze_start                           \
size_t nodes_count = parser->nodes.size;        \
size_t current_pointing = parser->data_pos;     \
struct node_st *expr_next = expr;               \
struct token_st *token = NULL;                  \
int result = SN_Status_Nothing, sub_result;

#define analyze_end_sub                                                             \
sub:        result = sub_result; goto end;                                          \
eof:        result = SN_Status_EOF; parser_set_error_token(parser, ErrorType_Syntax, "Unexpected end of file", parser->data_pos - 1); goto end; \
err:        result = SN_Status_Error; parser_set_error_token(parser, ErrorType_Syntax, "", parser->data_pos); goto end;

#define analyze_end                                 \
end:                                                \
if (result != SN_Status_Success) {                  \
node_clear(expr);                                   \
node_list_resize(&parser->nodes, nodes_count);      \
parser->data_pos = current_pointing;                \
} return result;                                    \
analyze_end_sub

#define check_call(call, check) {sub_result = call; if (sub_result == SN_Status_Nothing) check if (sub_result != SN_Status_Success) goto sub;}


int name_impr(struct parser_st *parser, struct node_st *expr) {
    parser_end {
        parser_set_error_token(parser, ErrorType_Syntax, "Unexpected end of file", parser->data_pos - 1);
        return SN_Status_EOF;
    }
    struct token_st *token;
    parser_get
    if (token->type != TokenType_Identifier) return SN_Status_Nothing;
    token_list_append(&expr->tokens, token);
    expr->type = MainType_Impr;
    expr->sub_type = ImprType_Name;
    parser->data_pos++;
    return SN_Status_Success;
}

int module_impr(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        check_call(name_impr(parser, expr_next), goto end;)
        while (parser->data_pos < parser->tokens.size) {
            parser_get
            if (token->type == TokenType_Special && token->sub_type == Special_DOT) {
                parser->data_pos++;
                parser_end goto eof;
                parser_get
                if (token->type != TokenType_Identifier) goto err;
                parser->data_pos++;
                token_list_append(&expr->tokens, token);
                continue;
            }
            break;
        }
        result = SN_Status_Success;
    }
analyze_end
}

int module_as_impr(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type == TokenType_Special && token->sub_type == Special_MUL) {
            parser->data_pos++;
            expr->type = MainType_Impr;
            expr->sub_type = ImprType_Module_All;
            result = SN_Status_Success;
            goto end;
        }

        check_call(module_impr(parser, expr), goto end;)
        result = SN_Status_Success;

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_AS) goto end;
        parser->data_pos++;

        expr_cast
        expr->type = MainType_Impr;
        expr->sub_type = ImprType_Module_As;

        parser_get
        if (token->type != TokenType_Identifier) goto err;
        parser->data_pos++;
        token_list_append(&expr->tokens, token);
    }
analyze_end
}

int from_impr(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_FROM) goto end;
        parser->data_pos++;


        expr_add
        check_call(module_impr(parser, expr_next), goto err;)

        expr->type = MainType_Impr;
        expr->sub_type = ImprType_From;

        expr_add
        check_call(import_impr(parser, expr_next), goto err;)

        result = SN_Status_Success;
    }
analyze_end
}

int import_impr(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_IMPORT) goto end;
        parser->data_pos++;

        expr->type = MainType_Impr;
        expr->sub_type = ImprType_Import;

        while (parser->data_pos < parser->tokens.size) {
            expr_add
            check_call(module_as_impr(parser, expr_next), goto err;)

            parser_end goto eof;
            parser_get
            if (token->type != TokenType_Special || token->sub_type != Special_COMMA) break;
            parser->data_pos++;
        }
        result = SN_Status_Success;
    }
analyze_end
}

#pragma clang diagnostic pop