#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include "an_analize.h"

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, AN_NODE_TYPE); \
an_node_set(obj->data, expr); an_node_clear(expr); list_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, AN_NODE_TYPE); \
list_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->position)
#define parser_get token_ptr = parser->list->data[parser->position];token = parser->list->data[parser->position]->data;
#define analyze_end if (!result) {an_node_clear(expr);parser->position = current_pointing;}
#define analyze_start size_t current_pointing = parser->position;struct an_node *expr_next = expr; \
struct tk_token *token = NULL;struct object_st *token_ptr = NULL;int result = 0;


void annotated_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        expr_add(expr)
        ident_new_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Annot;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_EQ) goto end;
        parser->position++;
        list_append(expr->tokens, token_ptr);

        expr_add(expr)
        or_test_oper(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        result = 1;
    }
    end:
    analyze_end
}
void assignment_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        expr_add(expr)
        primary_expr(parser, expr_next);
        if (expr_next->type != PrimType_Subscript && expr_next->type != PrimType_Ident_get &&
            expr_next->type != PrimType_Attrib)
            goto end;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Assign;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_Special || (
                token->subtype != Special_EQ_RSHIFT && token->subtype != Special_EQ_ADD &&
                token->subtype != Special_EQ_SUB && token->subtype != Special_EQ_MUL &&
                token->subtype != Special_EQ_DIV && token->subtype != Special_EQ_MOD &&
                token->subtype != Special_EQ_AND && token->subtype != Special_EQ_OR &&
                token->subtype != Special_EQ_XOR && token->subtype != Special_EQ_LSHIFT
                && token->subtype != Special_EQ))
            goto end;
        parser->position++;
        list_append(expr->tokens, token_ptr);

        while (parser->position < parser->list->size) {
            expr_add(expr)
            or_test_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end {
                result = 1;
                goto end;
            }
            parser_get
            if (token->type != TokenType_Special || (
                    token->subtype != Special_EQ_RSHIFT && token->subtype != Special_EQ_ADD &&
                    token->subtype != Special_EQ_SUB && token->subtype != Special_EQ_MUL &&
                    token->subtype != Special_EQ_DIV && token->subtype != Special_EQ_MOD &&
                    token->subtype != Special_EQ_AND && token->subtype != Special_EQ_OR &&
                    token->subtype != Special_EQ_XOR && token->subtype != Special_EQ_LSHIFT
                    && token->subtype != Special_EQ)) {
                result = 1;
                goto end;
            }
            parser->position++;
            list_append(expr->tokens, token_ptr);

            if (expr_next->type != PrimType_Subscript && expr_next->type != PrimType_Ident_get &&
                expr_next->type != PrimType_Attrib)
                goto end;
        }
    }
    end:
    analyze_end
}
void return_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_RETURN) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Return;

        parser_end goto end;
        parser_get
        if (token->type == TokenType_Special && token->subtype == Special_SEMI) {
            result = 1;
            goto end;
        }

        expr_add(expr)
        or_test_oper(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;
        result = 1;
    }
    end:
    analyze_end
}
void break_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_BREAK) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Break;
        result = 1;
    }
    end:
    analyze_end
}
void continue_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_CONTINUE) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Continue;
        result = 1;
    }
    end:
    analyze_end
}
void simple_stmt(struct sc_parser *parser, struct an_node *expr) {
    return_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    break_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    continue_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    annotated_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    assignment_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    or_test_oper(parser, expr);
}
void stmt_list(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            simple_stmt(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = StmtType_List;
                expr->main_type = MainType_Stmt;
                times = 1;
            }
            expr_add(expr)
        }
        result = 1;
    }
    end:
    analyze_end
}

void parameter_list(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        list_ident(parser, expr, Special_LSB, Special_RSB);
        if (expr->type != PrimType_List) goto end;
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Params;
        result = 1;
    }
    end:
    analyze_end
}
void extends_list(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        list_oper(parser, expr, Special_LSB, Special_RSB);
        if (expr->type != PrimType_List) goto end;
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Extends;
        result = 1;
    }
    end:
    analyze_end
}

void function_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Func;

        expr_add(expr)
        ident_new_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        parameter_list(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        result = 1;
    }
    end:
    analyze_end
}
void public_function_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_PUBLIC) goto end;
        parser->position++;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_PUB_Func;

        expr_add(expr)
        ident_new_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        parameter_list(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        result = 1;
    }
    end:
    analyze_end
}
void static_function_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_STATIC) goto end;
        parser->position++;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_STC_Func;

        expr_add(expr)
        ident_new_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        parameter_list(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        result = 1;
    }
    end:
    analyze_end
}
void private_function_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_PRIVATE) goto end;
        parser->position++;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_PRI_Func;

        expr_add(expr)
        ident_new_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        parameter_list(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        result = 1;
    }
    end:
    analyze_end
}

void if_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_IF) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_If;

        expr_add(expr)
        scopes_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        while (parser->position < parser->list->size) {
            parser_get
            if (token->type != TokenType_KeyWords || token->subtype != KeyWord_ELSE) break;
            parser->position++;

            parser_end goto end;
            parser_get
            if (token->type == TokenType_KeyWords && token->subtype == KeyWord_IF) {
                parser->position++;

                expr_add(expr)
                scopes_expr(parser, expr_next);
                if (expr_next->type == ExprType_None) goto end;

                expr_add(expr)
                suite(parser, expr_next);
                if (expr_next->type == ExprType_None) goto end;
            } else {
                expr_add(expr)
                suite(parser, expr_next);
                if (expr_next->type == ExprType_None) goto end;
            }
        }
        result = 1;
    }
    end:
    analyze_end
}
void while_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_WHILE) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_While;

        expr_add(expr)
        scopes_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;
        result = 1;
    }
    end:
    analyze_end
}
void do_while_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_DO) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_DoWhile;

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_WHILE) goto end;
        parser->position++;

        expr_add(expr)
        scopes_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        result = 1;
    }
    end:
    analyze_end
}
void class_stmt(struct sc_parser *parser, struct an_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_CLASS) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Class;

        expr_add(expr)
        ident_new_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        parser_get
        if (token->type == TokenType_Special && token->subtype == Special_LSB) {
            expr_add(expr)
            extends_list(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;
        }

        expr_add(expr)
        suite(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;

        result = 1;
    }
    end:
    analyze_end
}
void compound_stmt(struct sc_parser *parser, struct an_node *expr) {
    function_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    public_function_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    static_function_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    private_function_stmt(parser, expr);
    if (expr->type != ExprType_None) return;

    if_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    while_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    do_while_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    class_stmt(parser, expr);
}

void statement(struct sc_parser *parser, struct an_node *expr) {
    compound_stmt(parser, expr);
    if (expr->type != ExprType_None) return;
    analyze_start
    {
        stmt_list(parser, expr);

        parser_end goto end;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_SEMI) goto end;
        parser->position++;
        result = 1;
    }
    end:
    analyze_end
}
void suite(struct sc_parser *parser, struct an_node *expr) {
    statement(parser, expr);
    if (expr->type != ExprType_None) return;
    analyze_start
    {
        parser_end goto end;
        token = parser->list->data[parser->position]->data;
        if (token->type != TokenType_Special || token->subtype != Special_LCB) goto end;
        parser->position++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_List;

        parser_end goto end;
        token = parser->list->data[parser->position]->data;
        if (token->type == TokenType_Special && token->subtype == Special_RCB) {
            parser->position++;
            result = 1;
            goto end;
        }

        expr_add(expr)
        while (parser->position < parser->list->size) {
            parser_end break;
            parser_get
            if (token->type == TokenType_Special || token->subtype == Special_LCB) suite(parser, expr_next);
            else statement(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type == TokenType_Special && token->subtype == Special_RCB) {
                parser->position++;
                result = 1;
                goto end;
            }

            parser_end break;
            expr_add(expr)
        }
    }
    end:
    analyze_end
}
void token_analyzer(struct sc_parser *parser, struct an_node *expr) {
    parser->position = 0;
    analyze_start
    {
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_List;

        expr_add(expr)
        while (parser->position < parser->list->size) {
            suite(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;
            parser_end break;
            expr_add(expr)
        }
        result = 1;
    }
    end:
    analyze_end
}

#pragma clang diagnostic pop