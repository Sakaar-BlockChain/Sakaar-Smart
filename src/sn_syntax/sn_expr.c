#include "sn_syntax.h"
#include "hash_code.h"

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, AST_NODE_TYPE); \
ast_node_set(obj->data, expr); ast_node_clear(expr); list_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, AST_NODE_TYPE); \
list_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->pos)

char scopes_expr(struct sc_parser *parser, struct ast_node *expr) {
    size_t current_pointing = parser->pos;
    struct tk_token *token = NULL;
    char result = SN_Status_Nothing, sub_result;
    {
        parser_end goto end;
        token = parser->list->data[parser->pos]->data;
        if (token->type != TokenType_Special || token->subtype != Special_LSB) goto end;
        parser->pos++;

        sub_result = or_test_oper(parser, expr);
        if (sub_result != SN_Status_Success) goto end;

        parser_end goto end;
        token = parser->list->data[parser->pos]->data;
        if (token->type != TokenType_Special || token->subtype != Special_RSB) goto end;
        parser->pos++;

        result = SN_Status_Success;
    }
    end:
    if (result != SN_Status_Success) {
        ast_node_clear(expr);
        parser->pos = current_pointing;
    }
    return result;
}
char list_expr(struct sc_parser *parser, struct ast_node *expr) {
    return list_oper(parser, expr, Special_LSQB, Special_RSQB);
}
char ident_get_expr(struct sc_parser *parser, struct ast_node *expr) {
    parser_end return SN_Status_Nothing;
    struct tk_token *token = parser->list->data[parser->pos]->data;
    if (token->type != TokenType_Identifier) return SN_Status_Nothing;
    parser->pos++;
    expr->type = PrimType_Ident_get;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
    sha256_code._code(expr->data->data, expr->data->data);
    return SN_Status_Success;
}
char ident_new_expr(struct sc_parser *parser, struct ast_node *expr) {
    parser_end return SN_Status_Nothing;
    struct tk_token *token = parser->list->data[parser->pos]->data;
    if (token->type != TokenType_Identifier) return SN_Status_Nothing;
    parser->pos++;
    expr->type = PrimType_Ident_new;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
    sha256_code._code(expr->data->data, expr->data->data);
    return SN_Status_Success;
}
char bool_expr(struct sc_parser *parser, struct ast_node *expr) {
    parser_end return SN_Status_Nothing;
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
    parser_end return SN_Status_Nothing;
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
    parser_end return SN_Status_Nothing;
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
    parser_end return SN_Status_Nothing;
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
        sub_result = atom_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;
        while(parser->pos < parser->list->size){
            token = parser->list->data[parser->pos]->data;
            if (token->type == TokenType_Special && token->subtype == Special_DOT) {
                parser->pos++;

                expr_cast(expr)
                expr->type = PrimType_Attrib;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                parser_end goto end;
                token = parser->list->data[parser->pos]->data;
                if (token->type != TokenType_Identifier) goto end;
                parser->pos++;

                expr->data = object_new();
                object_set_type(expr->data, STRING_TYPE);
                string_set_str(expr->data->data, token->data, token->size);
                sha256_code._code(expr->data->data, expr->data->data);
                continue;
            } else if (token->type == TokenType_Special && token->subtype == Special_LSQB) {
                parser->pos++;

                expr_cast(expr)
                expr->type = PrimType_Subscript;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                sub_result = or_test_oper(parser, expr_next);
                if (sub_result != SN_Status_Success) goto end;

                parser_end goto end;
                token = parser->list->data[parser->pos]->data;
                if (token->type != TokenType_Special || token->subtype != Special_RSQB) goto end;
                parser->pos++;

                continue;
            } else if (token->type == TokenType_Special && token->subtype == Special_LSB) {
                expr_cast(expr)
                expr->type = PrimType_Call;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                sub_result = list_oper(parser, expr_next, Special_LSB, Special_RSB);
                if (sub_result != SN_Status_Success) goto end;

                continue;
            }
            break;
        }
        result = SN_Status_Success;
    }
    end:
    if (result != SN_Status_Success) {
        ast_node_clear(expr);
        parser->pos = current_pointing;
    }
    return result;
}