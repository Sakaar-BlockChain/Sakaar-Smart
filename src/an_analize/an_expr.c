#include "an_analize.h"
#include "hash_code.h"

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, AN_NODE_TYPE); \
an_node_set(obj->data, expr); an_node_clear(expr); list_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, AN_NODE_TYPE); \
list_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->position)

void scopes_expr(struct sc_parser *parser, struct an_node *expr) {
    size_t current_pointing = parser->position;
    struct tk_token *token = NULL;
    int result = 0;
    {
        parser_end goto end;
        token = parser->list->data[parser->position]->data;
        if (token->type != TokenType_Special || token->subtype != Special_LSB) goto end;
        parser->position++;

        or_test_oper(parser, expr);
        if (expr->type == ExprType_None) goto end;

        parser_end goto end;
        token = parser->list->data[parser->position]->data;
        if (token->type != TokenType_Special || token->subtype != Special_RSB) goto end;
        parser->position++;

        result = 1;
    }
    end:
    if (!result) {
        an_node_clear(expr);
        parser->position = current_pointing;
    }
}
void list_expr(struct sc_parser *parser, struct an_node *expr) {
    list_oper(parser, expr, Special_LSQB, Special_RSQB);
}
void ident_get_expr(struct sc_parser *parser, struct an_node *expr) {
    parser_end return;
    struct tk_token *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Identifier) return;
    parser->position++;
    expr->type = PrimType_Ident_get;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
    sha256_code._code(expr->data->data, expr->data->data);
}
void ident_new_expr(struct sc_parser *parser, struct an_node *expr) {
    parser_end return;
    struct tk_token *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Identifier) return;
    parser->position++;
    expr->type = PrimType_Ident_new;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
    sha256_code._code(expr->data->data, expr->data->data);
}
void bool_expr(struct sc_parser *parser, struct an_node *expr) {
    parser_end return;
    struct tk_token *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || (token->subtype != KeyWord_FALSE && token->subtype != KeyWord_TRUE))
        return;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, INTEGER_TYPE);
    if (token->subtype == KeyWord_FALSE) integer_set_ui(expr->data->data, 0);
    else integer_set_ui(expr->data->data, 1);
}
void number_expr(struct sc_parser *parser, struct an_node *expr) {
    parser_end return;
    struct tk_token *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_Int) return;
    parser->position++;
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
}
void string_expr(struct sc_parser *parser, struct an_node *expr) {
    parser_end return;
    struct tk_token *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_String) return;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
    object_set_type(expr->data, STRING_TYPE);
    string_set_str(expr->data->data, token->data, token->size);
}
void null_expr(struct sc_parser *parser, struct an_node *expr) {
    parser_end return;
    struct tk_token *token = parser->list->data[parser->position]->data;
    if (token->type != TokenType_KeyWords || token->subtype != KeyWord_NONE) return;
    parser->position++;
    expr->type = PrimType_Literal;
    expr->main_type = MainType_Expr;
    expr->data = object_new();
}
void literal_expr(struct sc_parser *parser, struct an_node *expr) {
    null_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    string_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    number_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    bool_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    list_expr(parser, expr);
}
void atom_expr(struct sc_parser *parser, struct an_node *expr) {
    ident_get_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    literal_expr(parser, expr);
    if (expr->type != ExprType_None) return;
    scopes_expr(parser, expr);
}
void primary_expr(struct sc_parser *parser, struct an_node *expr) {
    size_t current_pointing = parser->position;
    struct an_node *expr_next = expr;
    struct tk_token *token = NULL;
    int result = 0;
    {
        atom_expr(parser, expr_next);
        if (expr->type == ExprType_None) goto end;
        while(parser->position < parser->list->size){
            token = parser->list->data[parser->position]->data;
            if (token->type == TokenType_Special && token->subtype == Special_DOT) {
                parser->position++;

                expr_cast(expr)
                expr->type = PrimType_Attrib;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                parser_end return;
                token = parser->list->data[parser->position]->data;
                if (token->type != TokenType_Identifier) return;
                parser->position++;

                expr->data = object_new();
                object_set_type(expr->data, STRING_TYPE);
                string_set_str(expr->data->data, token->data, token->size);
                sha256_code._code(expr->data->data, expr->data->data);
                continue;
            } else if (token->type == TokenType_Special && token->subtype == Special_LSQB) {
                parser->position++;

                expr_cast(expr)
                expr->type = PrimType_Subscript;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                or_test_oper(parser, expr_next);
                if (expr_next->type == ExprType_None) goto end;

                parser_end goto end;
                token = parser->list->data[parser->position]->data;
                if (token->type != TokenType_Special || token->subtype != Special_RSQB) goto end;
                parser->position++;

                continue;
            } else if (token->type == TokenType_Special && token->subtype == Special_LSB) {
                expr_cast(expr)
                expr->type = PrimType_Call;
                expr->main_type = MainType_Expr;
                expr_add(expr)

                list_oper(parser, expr_next, Special_LSB, Special_RSB);
                if (expr_next->type == ExprType_None) goto end;

                continue;
            }
            break;
        }
        result = 1;
    }
    end:
    if (!result) {
        an_node_clear(expr);
        parser->position = current_pointing;
    }
}