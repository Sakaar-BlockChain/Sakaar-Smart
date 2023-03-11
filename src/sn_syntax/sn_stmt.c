#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include "sn_syntax.h"

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, AST_NODE_TYPE); \
ast_node_set(obj->data, expr); ast_node_clear(expr); list_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, AST_NODE_TYPE); \
list_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->pos)
#define parser_get token_ptr = parser->list->data[parser->pos];token = parser->list->data[parser->pos]->data;
#define analyze_end if (result != SN_Status_Success) {ast_node_clear(expr);parser->error_pos = parser->pos;parser->pos = current_pointing;} return result;
#define analyze_start size_t current_pointing = parser->pos;struct ast_node *expr_next = expr; \
struct tk_token *token = NULL;struct object_st *token_ptr = NULL;char result = SN_Status_Nothing, sub_result;


char annotated_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        expr_add(expr)
        sub_result = ident_new_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Annot;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_EQ) goto end;
        parser->pos++;
        list_append(expr->tokens, token_ptr);

        expr_add(expr)
        sub_result = or_test_oper(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char assignment_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        expr_add(expr)
        sub_result = primary_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;
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
        parser->pos++;
        list_append(expr->tokens, token_ptr);

        while (parser->pos < parser->list->size) {
            expr_add(expr)
            sub_result = or_test_oper(parser, expr_next);
            if (sub_result != SN_Status_Success) goto end;

            parser_end {
                result = SN_Status_Success;
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
                result = SN_Status_Success;
                goto end;
            }
            parser->pos++;
            list_append(expr->tokens, token_ptr);

            if (expr_next->type != PrimType_Subscript && expr_next->type != PrimType_Ident_get &&
                expr_next->type != PrimType_Attrib)
                goto end;
        }
    }
    end:
    analyze_end
}
char return_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_RETURN) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Return;

        parser_end goto end;
        parser_get
        if (token->type == TokenType_Special && token->subtype == Special_SEMI) {
            result = SN_Status_Success;
            goto end;
        }

        expr_add(expr)
        sub_result = or_test_oper(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;
        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char break_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_BREAK) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Break;
        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char continue_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_CONTINUE) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Continue;
        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char simple_stmt(struct sc_parser *parser, struct ast_node *expr) {
    char result = return_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = break_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = continue_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = annotated_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = assignment_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = or_test_oper(parser, expr);
    return result;
}
char stmt_list(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->pos < parser->list->size) {
            sub_result = simple_stmt(parser, expr_next);
            if (sub_result != SN_Status_Success) goto end;

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->pos++;

            parser_end goto end;
            if (times == 0) {
                expr_cast(expr)
                expr->type = StmtType_List;
                expr->main_type = MainType_Stmt;
                times = 1;
            }
            expr_add(expr)
        }
        result = SN_Status_Success;
    }
    end:
    analyze_end
}

char parameter_list(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        sub_result = list_ident(parser, expr, Special_LSB, Special_RSB);
        if (sub_result != SN_Status_Success) goto end;
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Params;
        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char extends_list(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        sub_result = list_oper(parser, expr, Special_LSB, Special_RSB);
        if (sub_result != SN_Status_Success) goto end;
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Extends;
        result = SN_Status_Success;
    }
    end:
    analyze_end
}

char function_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Func;

        expr_add(expr)
        sub_result = ident_new_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr_add(expr)
        sub_result = parameter_list(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr_add(expr)
        sub_result = suite(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char public_function_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_PUBLIC) goto end;
        parser->pos++;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_PUB_Func;

        expr_add(expr)
        sub_result = ident_new_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr_add(expr)
        sub_result = parameter_list(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr_add(expr)
        sub_result = suite(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char static_function_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_STATIC) goto end;
        parser->pos++;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_STC_Func;

        expr_add(expr)
        sub_result = ident_new_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr_add(expr)
        sub_result = parameter_list(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr_add(expr)
        sub_result = suite(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char private_function_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_PRIVATE) goto end;
        parser->pos++;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_FUNC) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_PRI_Func;

        expr_add(expr)
        sub_result = ident_new_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr_add(expr)
        sub_result = parameter_list(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr_add(expr)
        sub_result = suite(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        result = SN_Status_Success;
    }
    end:
    analyze_end
}

char if_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_IF) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_If;

        expr_add(expr)
        sub_result = scopes_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr_add(expr)
        sub_result = suite(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        while (parser->pos < parser->list->size) {
            parser_get
            if (token->type != TokenType_KeyWords || token->subtype != KeyWord_ELSE) break;
            parser->pos++;

            parser_end goto end;
            parser_get
            if (token->type == TokenType_KeyWords && token->subtype == KeyWord_IF) {
                parser->pos++;

                expr_add(expr)
                sub_result = scopes_expr(parser, expr_next);
                if (sub_result != SN_Status_Success) goto end;

                expr_add(expr)
                sub_result = suite(parser, expr_next);
                if (sub_result != SN_Status_Success) goto end;
            } else {
                expr_add(expr)
                sub_result = suite(parser, expr_next);
                if (sub_result != SN_Status_Success) goto end;
            }
        }
        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char while_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_WHILE) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_While;

        expr_add(expr)
        sub_result = scopes_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        expr_add(expr)
        sub_result = suite(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;
        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char do_while_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_DO) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_DoWhile;

        expr_add(expr)
        sub_result = suite(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_WHILE) goto end;
        parser->pos++;

        expr_add(expr)
        sub_result = scopes_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char class_stmt(struct sc_parser *parser, struct ast_node *expr) {
    analyze_start
    {
        parser_end goto end;
        parser_get
        if (token->type != TokenType_KeyWords || token->subtype != KeyWord_CLASS) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_Class;

        expr_add(expr)
        sub_result = ident_new_expr(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        parser_get
        if (token->type == TokenType_Special && token->subtype == Special_LSB) {
            expr_add(expr)
            sub_result = extends_list(parser, expr_next);
            if (sub_result != SN_Status_Success) goto end;
        }

        expr_add(expr)
        sub_result = suite(parser, expr_next);
        if (sub_result != SN_Status_Success) goto end;

        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char compound_stmt(struct sc_parser *parser, struct ast_node *expr) {
    char result = function_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = public_function_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = static_function_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = private_function_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;

    result = if_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = while_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = do_while_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = class_stmt(parser, expr);
    return result;
}

char statement(struct sc_parser *parser, struct ast_node *expr) {
    char res = compound_stmt(parser, expr);
    if (res != SN_Status_Nothing) return res;
    analyze_start
    {
        stmt_list(parser, expr);

        parser_end goto end;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_SEMI) goto end;
        parser->pos++;
        result = SN_Status_Success;
    }
    end:
    analyze_end
}
char suite(struct sc_parser *parser, struct ast_node *expr) {
    char res = statement(parser, expr);
    if (res != SN_Status_Nothing) return res;
    analyze_start
    {
        parser_end goto end;
        token = parser->list->data[parser->pos]->data;
        if (token->type != TokenType_Special || token->subtype != Special_LCB) goto end;
        parser->pos++;

        expr->main_type = MainType_Stmt;
        expr->type = StmtType_List;

        parser_end goto end;
        token = parser->list->data[parser->pos]->data;
        if (token->type == TokenType_Special && token->subtype == Special_RCB) {
            parser->pos++;
            result = SN_Status_Success;
            goto end;
        }

        expr_add(expr)
        while (parser->pos < parser->list->size) {
            parser_end break;
            parser_get
            if (token->type == TokenType_Special && token->subtype == Special_LCB) sub_result = suite(parser, expr_next);
            else sub_result = statement(parser, expr_next);
            if (sub_result != SN_Status_Success) goto end;

            parser_end break;
            parser_get
            if (token->type == TokenType_Special && token->subtype == Special_RCB) {
                parser->pos++;
                result = SN_Status_Success;
                goto end;
            }

            parser_end break;
            expr_add(expr)
        }
    }
    end:
    analyze_end
}
char token_analyzer(struct sc_parser *parser, struct ast_node *expr) {
    parser->pos = 0;
    analyze_start
    {
        expr->main_type = MainType_Stmt;
        expr->type = StmtType_List;

        expr_add(expr)
        while (parser->pos < parser->list->size) {
            sub_result = suite(parser, expr_next);
            if (sub_result != SN_Status_Success) goto end;
            parser_end break;
            expr_add(expr)
        }
        result = SN_Status_Success;
    }
    end:
    analyze_end
}

#pragma clang diagnostic pop