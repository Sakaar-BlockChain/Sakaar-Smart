#include "an_analize.h"
#include "tk_tokenize.h"

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, AN_NODE_TYPE); \
an_node_set(obj->data, expr); an_node_clear(expr); list_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, AN_NODE_TYPE); \
list_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->position)
#define parser_get token_ptr = parser->list->data[parser->position];token = parser->list->data[parser->position]->data;
#define analyze_end if (!result) {an_node_clear(expr);parser->position = current_pointing;}
#define analyze_start size_t current_pointing = parser->position;struct an_node *expr_next = expr; \
struct tk_token *token = NULL;struct object_st *token_ptr = NULL;int result = 0;

void u_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        while (parser->position < parser->list->size) {
            parser_get
            if (token->type != TokenType_Special || (
                    token->subtype != Special_ADD && token->subtype != Special_SUB)) break;
            parser->position++;

            if (token->subtype == Special_ADD) continue;
            list_append(expr->tokens, token_ptr);
        }
        if(expr->tokens->size % 2 != 0) {
            expr_add(expr)
            expr->type = ExprType_U;
            expr->main_type = MainType_Oper;
        }else an_node_clear(expr);
        primary_expr(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;
        result = 1;
    }
    end:
    analyze_end
}
void m_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            u_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || (
                    token->subtype != Special_MUL && token->subtype != Special_DIV &&
                    token->subtype != Special_MOD))
                break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_M;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = 1;
    }
    end:
    analyze_end
}
void a_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            m_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special ||
                (token->subtype != Special_ADD && token->subtype != Special_SUB))
                break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_A;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = 1;
    }
    end:
    analyze_end
}
void shift_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            a_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special ||
                (token->subtype != Special_LSHIFT && token->subtype != Special_RSHIFT))
                break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Shift;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = 1;
    }
    end:
    analyze_end
}
void and_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            shift_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_AND) break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_And;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = 1;
    }
    end:
    analyze_end
}
void xor_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            and_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_XOR) break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Xor;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = 1;
    }
    end:
    analyze_end
}
void or_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            xor_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_OR) break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Or;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = 1;
    }
    end:
    analyze_end
}

void comp_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            or_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || (
                    token->subtype != Special_EQ_NOT && token->subtype != Special_EQ_EQ &&
                    token->subtype != Special_EQ_LESS && token->subtype != Special_EQ_GREATER &&
                    token->subtype != Special_LESS && token->subtype != Special_GREATER))
                break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Comp;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = 1;
    }
    end:
    analyze_end
}
void not_test_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        while (parser->position < parser->list->size) {
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_NOT) break;
            parser->position++;

            if (token->subtype == Special_ADD) continue;
            list_append(expr->tokens, token_ptr);
        }
        if(expr->tokens->size % 2 != 0) {
            expr_add(expr)
            expr->type = ExprType_NotTest;
            expr->main_type = MainType_Oper;
        }else an_node_clear(expr);
        comp_oper(parser, expr_next);
        if (expr_next->type == ExprType_None) goto end;
        result = 1;
    }
    end:
    analyze_end
}
void and_test_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            not_test_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_AND_AND) break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_AndTest;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = 1;
    }
    end:
    analyze_end
}
void or_test_oper(struct an_parser *parser, struct an_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            and_test_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_OR_OR) break;
            parser->position++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_OrTest;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = 1;
    }
    end:
    analyze_end
}
void list_oper(struct an_parser *parser, struct an_node *expr, short start, short end) {
    analyze_start
    {
        parser_end goto end;
        token = parser->list->data[parser->position]->data;
        if (token->type != TokenType_Special || token->subtype != start) goto end;
        parser->position++;

        expr->main_type = MainType_Expr;
        expr->type = PrimType_List;

        parser_end goto end;
        token = parser->list->data[parser->position]->data;
        if (token->type == TokenType_Special && token->subtype == end) {
            parser->position++;
            result = 1;
            goto end;
        }

        expr_add(expr)
        while (parser->position < parser->list->size) {
            and_test_oper(parser, expr_next);
            if (expr_next->type == ExprType_None) goto end;

            parser_end break;
            parser_get
            if (token->type == TokenType_Special && token->subtype == end) {
                parser->position++;
                result = 1;
                goto end;
            }
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->position++;

            parser_end break;
            expr_add(expr)
        }
    }
    end:
    analyze_end
}