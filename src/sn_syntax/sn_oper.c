#include "sn_syntax.h"

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, AST_NODE_TYPE); \
ast_node_set(obj->data, expr); ast_node_clear(expr); list_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, AST_NODE_TYPE); \
list_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->pos)
#define parser_get token_ptr = parser->list->data[parser->pos];token = parser->list->data[parser->pos]->data;
#define analyze_start size_t current_pointing = parser->pos;struct ast_node *expr_next = expr; struct tk_token *token = NULL;struct object_st *token_ptr = NULL;char result = SN_Status_Nothing, sub_result;
#define analyze_end \
end:    if (result != SN_Status_Success) {ast_node_clear(expr);parser->pos = current_pointing;} return result; \
sub:    result = sub_result; goto end; \
eof:    result = SN_Status_EOF; parser->error_pos = parser->pos; goto end; \
err:    result = SN_Status_Error; parser->error_pos = parser->pos; goto end;

#define check_call(call, check) {sub_result = call; if (sub_result == SN_Status_Nothing) check if (sub_result != SN_Status_Success) goto sub;}

char u_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        while (parser->pos < parser->list->size) {
            parser_get
            if (token->type != TokenType_Special || (
                    token->subtype != Special_ADD && token->subtype != Special_SUB))
                break;
            parser->pos++;

            if (token->subtype == Special_ADD) continue;
            list_append(expr->tokens, token_ptr);
        }
        if (expr->tokens->size % 2 != 0) {
            expr_add(expr)
            expr->type = ExprType_U;
            expr->main_type = MainType_Oper;
        } else ast_node_clear(expr);
        check_call(primary_expr(parser, expr_next), {
            if (expr_next == expr) goto end;
            else goto err;
        })
        result = SN_Status_Success;
    }
analyze_end
}
char m_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->pos < parser->list->size) {
            check_call(u_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || (
                    token->subtype != Special_MUL && token->subtype != Special_DIV &&
                    token->subtype != Special_MOD))
                break;
            parser->pos++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_M;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
char a_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->pos < parser->list->size) {
            check_call(m_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special ||
                (token->subtype != Special_ADD && token->subtype != Special_SUB))
                break;
            parser->pos++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_A;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
char shift_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->pos < parser->list->size) {
            check_call(a_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special ||
                (token->subtype != Special_LSHIFT && token->subtype != Special_RSHIFT))
                break;
            parser->pos++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Shift;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
char and_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->pos < parser->list->size) {
            check_call(shift_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_AND) break;
            parser->pos++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_And;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
char xor_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->pos < parser->list->size) {
            check_call(and_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_XOR) break;
            parser->pos++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Xor;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
char or_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->pos < parser->list->size) {
            check_call(xor_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_OR) break;
            parser->pos++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Or;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}

char comp_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->pos < parser->list->size) {
            check_call(or_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || (
                    token->subtype != Special_EQ_NOT && token->subtype != Special_EQ_EQ &&
                    token->subtype != Special_EQ_LESS && token->subtype != Special_EQ_GREATER &&
                    token->subtype != Special_LESS && token->subtype != Special_GREATER))
                break;
            parser->pos++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Comp;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
char not_test_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        while (parser->pos < parser->list->size) {
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_NOT) break;
            parser->pos++;

            if (token->subtype == Special_ADD) continue;
            list_append(expr->tokens, token_ptr);
        }
        if (expr->tokens->size % 2 != 0) {
            expr_add(expr)
            expr->type = ExprType_NotTest;
            expr->main_type = MainType_Oper;
        } else ast_node_clear(expr);
        check_call(comp_oper(parser, expr_next), {
            if (expr_next == expr) goto end;
            else goto err;
        })
        result = SN_Status_Success;
    }
analyze_end
}
char and_test_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->pos < parser->list->size) {
            check_call(not_test_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_AND_AND) break;
            parser->pos++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_AndTest;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
char or_test_oper(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->pos < parser->list->size) {
            check_call(and_test_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_OR_OR) break;
            parser->pos++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_OrTest;
                expr->main_type = MainType_Oper;
                times = 1;
            }
            expr_add(expr)
            list_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
char list_oper(struct sc_parser *parser, struct ast_node *expr, short start, short end) {
    analyze_start
    {
        parser_end goto eof;
        token = parser->list->data[parser->pos]->data;
        if (token->type != TokenType_Special || token->subtype != start) goto end;
        parser->pos++;

        expr->main_type = MainType_Expr;
        expr->type = PrimType_List;

        parser_end goto eof;
        token = parser->list->data[parser->pos]->data;
        if (token->type == TokenType_Special && token->subtype == end) {
            parser->pos++;
            result = SN_Status_Success;
            goto end;
        }

        expr_add(expr)
        while (parser->pos < parser->list->size) {
            check_call(or_test_oper(parser, expr_next), goto err;)

            parser_end break;
            parser_get
            if (token->type == TokenType_Special && token->subtype == end) {
                parser->pos++;
                result = SN_Status_Success;
                goto end;
            }
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->pos++;

            parser_end break;
            expr_add(expr)
        }
    }
analyze_end
}

char list_ident(struct sc_parser *parser, struct ast_node *expr, short start, short end) {
    analyze_start
    {
        parser_end goto eof;
        token = parser->list->data[parser->pos]->data;
        if (token->type != TokenType_Special || token->subtype != start) goto end;
        parser->pos++;

        expr->main_type = MainType_Expr;
        expr->type = PrimType_List;

        parser_end goto eof;
        token = parser->list->data[parser->pos]->data;
        if (token->type == TokenType_Special && token->subtype == end) {
            parser->pos++;
            result = SN_Status_Success;
            goto end;
        }

        expr_add(expr)
        while (parser->pos < parser->list->size) {
            check_call(ident_new_expr(parser, expr_next), goto err;)

            parser_end break;
            parser_get
            if (token->type == TokenType_Special && token->subtype == end) {
                parser->pos++;
                result = SN_Status_Success;
                goto end;
            }
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->pos++;

            parser_end break;
            expr_add(expr)
        }
    }
analyze_end
}