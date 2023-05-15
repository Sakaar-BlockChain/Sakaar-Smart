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


int scopes_expr(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || token->sub_type != Special_LSB) goto end;
        parser->data_pos++;

        check_call(or_test_oper(parser, expr), goto err;)

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || token->sub_type != Special_RSB) goto err;
        parser->data_pos++;

        result = SN_Status_Success;
    }

analyze_end
}

int list_expr(struct parser_st *parser, struct node_st *expr) {
    return list_oper(parser, expr, Special_LSQB, Special_RSQB);
}

int ident_get_expr(struct parser_st *parser, struct node_st *expr) {
    parser_end {
        parser_set_error_token(parser, ErrorType_Syntax, "Unexpected end of file", parser->data_pos - 1);
        return SN_Status_EOF;
    }
    struct token_st *token;
    parser_get
    if (token->type != TokenType_Identifier) return SN_Status_Nothing;
    expr->sub_type = PrimType_Ident_get;
    expr->type = MainType_Expr;
    expr->data = parser_get_ident(parser, &token->data);
    if (expr->data == 0) {
        node_clear(expr);
        parser_set_error_token(parser, ErrorType_Semantic, "Undefined variable", parser->data_pos);
        return SN_Status_Error;
    }
    parser->data_pos++;
    return SN_Status_Success;
}

int ident_new_expr(struct parser_st *parser, struct node_st *expr) {
    parser_end {
        parser_set_error_token(parser, ErrorType_Syntax, "Unexpected end of file", parser->data_pos - 1);
        return SN_Status_EOF;
    }
    struct token_st *token;
    parser_get
    if (token->type != TokenType_Identifier) return SN_Status_Nothing;
    parser->data_pos++;
    expr->sub_type = PrimType_Ident_new;
    expr->type = MainType_Expr;
    expr->data = parser_new_ident(parser, &token->data);
    return SN_Status_Success;
}

int bool_expr(struct parser_st *parser, struct node_st *expr) {
    parser_end {
        parser_set_error_token(parser, ErrorType_Syntax, "Unexpected end of file", parser->data_pos - 1);
        return SN_Status_EOF;
    }
    struct token_st *token;
    parser_get
    if (token->type != TokenType_KeyWords || (token->sub_type != KeyWord_FALSE && token->sub_type != KeyWord_TRUE))
        return SN_Status_Nothing;
    parser->data_pos++;
    expr->sub_type = PrimType_Literal;
    expr->type = MainType_Expr;
    {
        struct object_st *obj = object_new();
        object_set_type(obj, INTEGER_TYPE);
        if (token->sub_type == KeyWord_FALSE) integer_set_ui(obj->data, 0);
        else integer_set_ui(obj->data, 1);
        expr->data = parser_const_obj(parser, obj);
    }
    return SN_Status_Success;
}

int number_expr(struct parser_st *parser, struct node_st *expr) {
    parser_end {
        parser_set_error_token(parser, ErrorType_Syntax, "Unexpected end of file", parser->data_pos - 1);
        return SN_Status_EOF;
    }
    struct token_st *token;
    parser_get
    if (token->type != TokenType_Int) return SN_Status_Nothing;
    parser->data_pos++;
    expr->sub_type = PrimType_Literal;
    expr->type = MainType_Expr;
    {
        struct object_st *obj = object_new();
        switch (token->sub_type) {
            case IntType_bin: {
                object_set_type(obj, INTEGER_TYPE);
                integer_set_str_bin_(obj->data, token->data.data, token->data.size);
                break;
            }
            case IntType_oct: {
                object_set_type(obj, INTEGER_TYPE);
                integer_set_str_oct_(obj->data, token->data.data, token->data.size);
                break;
            }
            case IntType_dec: {
                object_set_type(obj, INTEGER_TYPE);
                integer_set_str_dec_(obj->data, token->data.data, token->data.size);
                break;
            }
            case IntType_hex: {
                object_set_type(obj, INTEGER_TYPE);
                integer_set_str_(obj->data, token->data.data, token->data.size);
                break;
            }
            case IntType_float: {
                object_set_type(obj, FLOAT_TYPE);
                // TODO get float from string
                break;
            }
        }
        expr->data = parser_const_obj(parser, obj);
    }
    return SN_Status_Success;
}

int string_expr(struct parser_st *parser, struct node_st *expr) {
    parser_end {
        parser_set_error_token(parser, ErrorType_Syntax, "Unexpected end of file", parser->data_pos - 1);
        return SN_Status_EOF;
    }
    struct token_st *token;
    parser_get
    if (token->type != TokenType_String) return SN_Status_Nothing;
    parser->data_pos++;
    expr->sub_type = PrimType_Literal;
    expr->type = MainType_Expr;
    {
        struct object_st *obj = object_new();
        object_set_type(obj, STRING_TYPE);
        string_set(obj->data, &token->data);
        expr->data = parser_const_obj(parser, obj);
    }
    return SN_Status_Success;
}

int null_expr(struct parser_st *parser, struct node_st *expr) {
    parser_end {
        parser_set_error_token(parser, ErrorType_Syntax, "Unexpected end of file", parser->data_pos - 1);
        return SN_Status_EOF;
    }
    struct token_st *token;
    parser_get
    if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_NONE) return SN_Status_Nothing;
    parser->data_pos++;
    expr->sub_type = PrimType_Literal;
    expr->type = MainType_Expr;
    expr->data = 0;
    return SN_Status_Success;
}

int literal_expr(struct parser_st *parser, struct node_st *expr) {
    int result = null_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = string_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = number_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = bool_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = list_expr(parser, expr);
    return result;
}

int atom_expr(struct parser_st *parser, struct node_st *expr) {
    int result = ident_get_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = literal_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = scopes_expr(parser, expr);
    return result;
}

int primary_expr(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        check_call(atom_expr(parser, expr_next), goto end;)
        while (parser->data_pos < parser->tokens.size) {
            parser_get
            if (token->type == TokenType_Special && token->sub_type == Special_DOT) {
                parser->data_pos++;

                expr_cast
                expr->sub_type = PrimType_Attrib;
                expr->type = MainType_Expr;

                parser_end goto eof;
                parser_get
                if (token->type != TokenType_Identifier) goto err;
                parser->data_pos++;

                {
                    struct object_st *obj = object_new();
                    object_set_type(obj, STRING_TYPE);
                    string_set_str(obj->data, token->data.data, token->data.size);
                    expr->data = parser_const_obj(parser, obj);
                }
                continue;
            } else if (token->type == TokenType_Special && token->sub_type == Special_LSQB) {
                parser->data_pos++;

                expr_cast
                expr->sub_type = PrimType_Subscript;
                expr->type = MainType_Expr;
                expr_add

                check_call(or_test_oper(parser, expr_next), goto err;)

                parser_end goto eof;
                parser_get
                if (token->type != TokenType_Special || token->sub_type != Special_RSQB) goto err;
                parser->data_pos++;

                continue;
            } else if (token->type == TokenType_Special && token->sub_type == Special_LSB) {
                expr_cast
                expr->sub_type = PrimType_Call;
                expr->type = MainType_Expr;
                expr_add

                check_call(list_oper(parser, expr_next, Special_LSB, Special_RSB), goto err;)

                continue;
            }
            break;
        }
        result = SN_Status_Success;
    }
analyze_end
}