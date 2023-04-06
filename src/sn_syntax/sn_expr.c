#include "sn_syntax.h"

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, AST_NODE_TYPE); \
ast_node_set(obj->data, expr); ast_node_clear(expr); list_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, AST_NODE_TYPE); \
list_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->pos)
#define analyze_end \
end:    if (result != SN_Status_Success) {ast_node_clear(expr);parser->pos = current_pointing;} return result; \
sub:    result = sub_result; goto end; \
eof:    result = SN_Status_EOF; parser->error_pos = parser->pos; goto end; \
err:    result = SN_Status_Error; parser->error_pos = parser->pos; goto end;

#define check_call(call, check) {sub_result = call; if (sub_result == SN_Status_Nothing) check if (sub_result != SN_Status_Success) goto sub;}

char scopes_expr(struct sc_parser *parser, struct ast_node *expr) {
    size_t current_pointing = parser->pos;
    struct tk_token *token = NULL;
    char result = SN_Status_Nothing, sub_result;
    {
        parser_end goto eof;
        token = parser->list->data[parser->pos]->data;
        if (token->type != TokenType_Special || token->subtype != Special_LSB) goto end;
        parser->pos++;

        check_call(or_test_oper(parser, expr), goto err;)

        parser_end goto eof;
        token = parser->list->data[parser->pos]->data;
        if (token->type != TokenType_Special || token->subtype != Special_RSB) goto err;
        parser->pos++;

        result = SN_Status_Success;
    }
analyze_end
}
char list_expr(struct sc_parser *parser, struct ast_node *expr) {
    return list_oper(parser, expr, Special_LSQB, Special_RSQB);
}
char ident_get_expr(struct sc_parser *parser, struct ast_node *expr) {
    parser_end return SN_Status_EOF;
    struct tk_token *token = parser->list->data[parser->pos]->data;
    if (token->type != TokenType_Identifier) return SN_Status_Nothing;
    parser->pos++;
    expr->type = PrimType_Ident_get;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
    return SN_Status_Success;
}
char ident_new_expr(struct sc_parser *parser, struct ast_node *expr) {
    parser_end return SN_Status_EOF;
    struct tk_token *token = parser->list->data[parser->pos]->data;
    if (token->type != TokenType_Identifier) return SN_Status_Nothing;
    parser->pos++;
    expr->type = PrimType_Ident_new;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
    return SN_Status_Success;
}
char bool_expr(struct sc_parser *parser, struct ast_node *expr) {
    parser_end return SN_Status_EOF;
    struct tk_token *token = parser->list->data[parser->pos]->data;
    if (token->type != TokenType_KeyWords || (token->subtype != KeyWord_FALSE && token->subtype != KeyWord_TRUE))
        return SN_Status_Nothing;
    parser->pos++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, INTEGER_TYPE);
    if (token->subtype == KeyWord_FALSE) integer_set_ui(expr->data->data, 0);
    else integer_set_ui(expr->data->data, 1);
    return SN_Status_Success;
}
char number_expr(struct sc_parser *parser, struct ast_node *expr) {
    parser_end return SN_Status_EOF;
    struct tk_token *token = parser->list->data[parser->pos]->data;
    if (token->type != TokenType_Int) return SN_Status_Nothing;
    parser->pos++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();

    switch(token->subtype){
        case IntType_bin: {
            object_set_type(expr->data, INTEGER_TYPE);
            _integer_set_str_bin(expr->data->data, token->data, token->size);
            break;
        }
        case IntType_oct: {
            object_set_type(expr->data, INTEGER_TYPE);
            _integer_set_str_oct(expr->data->data, token->data, token->size);
            break;
        }
        case IntType_dec: {
            object_set_type(expr->data, INTEGER_TYPE);
            _integer_set_str_dec(expr->data->data, token->data, token->size);
            break;
        }
        case IntType_hex: {
            object_set_type(expr->data, INTEGER_TYPE);
            _integer_set_str(expr->data->data, token->data, token->size);
            break;
        }
        case IntType_float: {
            object_set_type(expr->data, FLOAT_TYPE);
            // TODO get float from string
            break;
        }
    }
    return SN_Status_Success;
}
char string_expr(struct sc_parser *parser, struct ast_node *expr) {
    parser_end return SN_Status_EOF;
    struct tk_token *token = parser->list->data[parser->pos]->data;
    if (token->type != TokenType_String) return SN_Status_Nothing;
    parser->pos++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
    return SN_Status_Success;
}
char null_expr(struct sc_parser *parser, struct ast_node *expr) {
    parser_end return SN_Status_EOF;
    struct tk_token *token = parser->list->data[parser->pos]->data;
    if (token->type != TokenType_KeyWords || token->subtype != KeyWord_NONE) return SN_Status_Nothing;
    parser->pos++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    return SN_Status_Success;
}
char literal_expr(struct sc_parser *parser, struct ast_node *expr) {
    char result = null_expr(parser, expr);
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
char atom_expr(struct sc_parser *parser, struct ast_node *expr) {
    char result = ident_get_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = literal_expr(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = scopes_expr(parser, expr);
    return result;
}
char primary_expr(struct sc_parser *parser, struct ast_node *expr) {
    size_t current_pointing = parser->pos;
    struct ast_node *expr_next = expr;
    struct tk_token *token = NULL;
    char result = SN_Status_Nothing, sub_result;
    {
        check_call(atom_expr(parser, expr_next), goto end;)
        while(parser->pos < parser->list->size){
            token = parser->list->data[parser->pos]->data;
            if (token->type == TokenType_Special && token->subtype == Special_DOT) {
                parser->pos++;

                expr_cast(expr)
                expr->type = PrimType_Attrib;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                parser_end goto eof;
                token = parser->list->data[parser->pos]->data;
                if (token->type != TokenType_Identifier) goto err;
                parser->pos++;

                expr->data = object_new();
                object_set_type(expr->data, STRING_TYPE);
                string_set_str(expr->data->data, token->data, token->size);
                continue;
            } else if (token->type == TokenType_Special && token->subtype == Special_LSQB) {
                parser->pos++;

                expr_cast(expr)
                expr->type = PrimType_Subscript;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                check_call(or_test_oper(parser, expr_next), goto err;)

                parser_end goto eof;
                token = parser->list->data[parser->pos]->data;
                if (token->type != TokenType_Special || token->subtype != Special_RSQB) goto err;
                parser->pos++;

                continue;
            } else if (token->type == TokenType_Special && token->subtype == Special_LSB) {
                expr_cast(expr)
                expr->type = PrimType_Call;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                check_call(list_oper(parser, expr_next, Special_LSB, Special_RSB), goto err;)

                continue;
            }
            break;
        }
        result = SN_Status_Success;
    }
analyze_end
}