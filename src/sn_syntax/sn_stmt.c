#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include "sn_syntax.h"

#define expr_cast { \
node_list_add_new(&parser->nodes);          \
expr_next = node_list_last(&parser->nodes); \
node_set(expr_next, expr); node_clear(expr);\
node_list_append(&expr->nodes, expr_next);  \
}
#define expr_add { \
node_list_add_new(&parser->nodes);          \
expr_next = node_list_last(&parser->nodes); \
node_list_append(&(expr)->nodes, expr_next);  \
}

#define parser_end if (parser->tokens.size <= parser->data_pos)
#define parser_get token = parser->tokens.tokens[parser->data_pos];

#define analyze_start \
size_t nodes_count = parser->nodes.size;    \
size_t current_pointing = parser->data_pos; \
struct node_st *expr_next = expr;           \
struct token_st *token = NULL;              \
int result = SN_Status_Nothing, sub_result;

#define analyze_end_sub \
sub:        result = sub_result; goto end; \
eof:        result = SN_Status_EOF; parser->error_pos = parser->data_pos; goto end; \
err:        result = SN_Status_Error; parser->error_pos = parser->data_pos; goto end;
#define analyze_end \
end:                \
if (result != SN_Status_Success) {node_clear(expr); node_list_resize(&parser->nodes, nodes_count); parser->data_pos = current_pointing;} return result; \
analyze_end_sub

#define check_call(call, check) {sub_result = call; if (sub_result == SN_Status_Nothing) check if (sub_result != SN_Status_Success) goto sub;}


int annotated_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_VAR) goto end;
        parser->data_pos++;

        expr_add
        check_call(ident_new_expr(parser, expr_next), goto err;)

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Annot;
        result = SN_Status_Success;

        parser_end goto end;
        parser_get
        if (token->type != TokenType_Special || token->sub_type != Special_EQ) goto end;
        parser->data_pos++;

        expr_add
        check_call(or_test_oper(parser, expr_next), goto err;)
    }
analyze_end
}

int assignment_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        expr_add
        check_call(primary_expr(parser, expr_next), goto end;)
        if (expr_next->sub_type != PrimType_Subscript && expr_next->sub_type != PrimType_Ident_get &&
            expr_next->sub_type != PrimType_Attrib)
            goto end;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Assign;

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || (
                token->sub_type != Special_EQ_RSHIFT && token->sub_type != Special_EQ_ADD &&
                token->sub_type != Special_EQ_SUB && token->sub_type != Special_EQ_MUL &&
                token->sub_type != Special_EQ_DIV && token->sub_type != Special_EQ_MOD &&
                token->sub_type != Special_EQ_AND && token->sub_type != Special_EQ_OR &&
                token->sub_type != Special_EQ_XOR && token->sub_type != Special_EQ_LSHIFT
                && token->sub_type != Special_EQ))
            goto end;
        parser->data_pos++;
        token_list_append(&expr->tokens, token);

        while (parser->data_pos < parser->tokens.size) {
            expr_add
            check_call(or_test_oper(parser, expr_next), goto err;)

            parser_end {
                result = SN_Status_Success;
                goto end;
            }
            parser_get
            if (token->type != TokenType_Special || (
                    token->sub_type != Special_EQ_RSHIFT && token->sub_type != Special_EQ_ADD &&
                    token->sub_type != Special_EQ_SUB && token->sub_type != Special_EQ_MUL &&
                    token->sub_type != Special_EQ_DIV && token->sub_type != Special_EQ_MOD &&
                    token->sub_type != Special_EQ_AND && token->sub_type != Special_EQ_OR &&
                    token->sub_type != Special_EQ_XOR && token->sub_type != Special_EQ_LSHIFT
                    && token->sub_type != Special_EQ)) {
                result = SN_Status_Success;
                goto end;
            }
            parser->data_pos++;
            token_list_append(&expr->tokens, token);

            if (expr_next->sub_type != PrimType_Subscript && expr_next->sub_type != PrimType_Ident_get &&
                expr_next->sub_type != PrimType_Attrib)
                goto err;
        }
        goto err;
    }
analyze_end
}

int return_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_RETURN) goto end;
        if ((parser->scope_type & ScopeType_Func) != ScopeType_Func) goto err_func;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Return;

        parser_end goto eof;
        parser_get
        if (token->type == TokenType_Special && token->sub_type == Special_SEMI) {
            result = SN_Status_Success;
            goto end;
        }

        expr_add
        check_call(or_test_oper(parser, expr_next), goto err;)
        result = SN_Status_Success;
    }
analyze_end
    err_func:
    result = SM_Status_Error_Func;
    parser->error_pos = parser->data_pos;
    goto end;
}

int break_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_BREAK) goto end;
        if ((parser->scope_type & ScopeType_Loop) != ScopeType_Loop) goto err_loop;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Break;
        result = SN_Status_Success;
    }
analyze_end
    err_loop:
    result = SM_Status_Error_Loop;
    parser->error_pos = parser->data_pos;
    goto end;
}

int continue_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_CONTINUE) goto end;
        if ((parser->scope_type & ScopeType_Loop) != ScopeType_Loop) goto err_loop;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Continue;
        result = SN_Status_Success;
    }
analyze_end
    err_loop:
    result = SM_Status_Error_Loop;
    parser->error_pos = parser->data_pos;
    goto end;
}

int simple_oper(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        expr_add
        check_call(or_test_oper(parser, expr_next), goto end;)
        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Oper;
        result = SN_Status_Success;
    }
analyze_end
}

int simple_stmt(struct parser_st *parser, struct node_st *expr) {
    int result = return_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = break_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = continue_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = annotated_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = assignment_stmt(parser, expr);
    if (result != SN_Status_Nothing) return result;
    result = simple_oper(parser, expr);
    return result;
}

int stmt_list(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->data_pos < parser->tokens.size) {
            check_call(simple_stmt(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->sub_type != Special_COMMA) break;
            parser->data_pos++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast
                expr->sub_type = StmtType_List;
                expr->type = MainType_Stmt;
                times = 1;
            }
            expr_add
        }
        result = SN_Status_Success;
    }
analyze_end
}

int parameter_list(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        check_call(list_ident(parser, expr), goto end;)
        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Params;
        result = SN_Status_Success;
    }
analyze_end
}

int extends_list(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        check_call(list_oper(parser, expr, Special_LSB, Special_RSB), goto end;)
        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Extends;
        result = SN_Status_Success;
    }
analyze_end
}

int function_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_FUNC) goto end;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Func;

        expr_add
        check_call(ident_new_expr(parser, expr_next), goto err;)

        expr_add
        check_call(function_body_stmt(parser, expr_next), goto err;)

        result = SN_Status_Success;
    }
analyze_end
}

int public_function_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_PUBLIC) goto end;
        if ((parser->scope_type & ScopeType_Class) != ScopeType_Class) goto err_class;
        parser->data_pos++;

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_FUNC) goto err;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_PUB_Func;

        expr_add
        check_call(ident_new_expr(parser, expr_next), goto err;)

        expr_add
        check_call(function_body_stmt(parser, expr_next), goto err;)

        result = SN_Status_Success;
    }
analyze_end
    err_class:
    result = SM_Status_Error_Class;
    parser->error_pos = parser->data_pos;
    goto end;
}

int static_function_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_STATIC) goto end;
        if ((parser->scope_type & ScopeType_Class) != ScopeType_Class) goto err_class;
        parser->data_pos++;

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_FUNC) goto err;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_STC_Func;

        expr_add
        check_call(ident_new_expr(parser, expr_next), goto err;)

        expr_add
        check_call(function_body_stmt(parser, expr_next), goto err;)

        result = SN_Status_Success;
    }
analyze_end
    err_class:
    result = SM_Status_Error_Class;
    parser->error_pos = parser->data_pos;
    goto end;
}

int private_function_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_PRIVATE) goto end;
        if ((parser->scope_type & ScopeType_Class) != ScopeType_Class) goto err_class;
        parser->data_pos++;

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_FUNC) goto err;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_PRI_Func;

        expr_add
        check_call(ident_new_expr(parser, expr_next), goto err;)

        expr_add
        check_call(function_body_stmt(parser, expr_next), goto err;)

        result = SN_Status_Success;
    }
analyze_end
    err_class:
    result = SM_Status_Error_Class;
    parser->error_pos = parser->data_pos;
    goto end;
}

int function_body_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    size_t variable_count = parser->variables.size;
    size_t variable_stack_size = parser->variables_stack.size;
    size_t closure_count = parser->closures.size;
    size_t closure_stack_size = parser->closures_stack.size;
    {
        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Func_Body;

        {
            variable_list_add_new(&parser->variables);
            expr->variable = variable_list_last(&parser->variables);
            variable_list_append(&parser->variables_stack, expr->variable);
            expr->closure = closure_new();
            closure_list_append(&parser->closures, expr->closure);
            closure_list_append(&parser->closures_stack, expr->closure);
        }

        expr_add
        check_call(parameter_list(parser, expr_next), goto err;)

        expr_add
        check_call(suite(parser, expr_next, ScopeType_Func), goto err;)

        result = SN_Status_Success;
    }
    variable_list_resize(&parser->variables_stack, variable_stack_size);
    closure_list_resize(&parser->closures_stack, closure_stack_size);
    if (result != SN_Status_Success) {
        node_clear(expr);
        node_list_resize(&parser->nodes, nodes_count);
        closure_list_resize(&parser->closures, closure_count);
        variable_list_resize(&parser->variables, variable_count);
        parser->data_pos = current_pointing;
    }
    return result;
analyze_end
}

int if_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_IF) goto end;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_If;

        expr_add
        check_call(scopes_expr(parser, expr_next), goto err;)

        expr_add
        check_call(suite(parser, expr_next, parser->scope_type), goto err;)

        while (parser->data_pos < parser->tokens.size) {
            parser_get
            if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_ELSE) break;
            parser->data_pos++;

            parser_end goto eof;
            parser_get
            if (token->type == TokenType_KeyWords && token->sub_type == KeyWord_IF) {
                parser->data_pos++;

                expr_add
                check_call(scopes_expr(parser, expr_next), goto err;)

                expr_add
                check_call(suite(parser, expr_next, parser->scope_type), goto err;)
            } else {
                expr_add
                check_call(suite(parser, expr_next, parser->scope_type), goto err;)
            }
        }
        result = SN_Status_Success;
    }
analyze_end
}

int while_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_WHILE) goto end;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_While;

        expr_add
        check_call(scopes_expr(parser, expr_next), goto err;)

        expr_add
        check_call(suite(parser, expr_next, (parser->scope_type | ScopeType_Loop)), goto err;)
        result = SN_Status_Success;
    }
analyze_end
}

int do_while_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_DO) goto end;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_DoWhile;

        expr_add
        check_call(suite(parser, expr_next, (parser->scope_type | ScopeType_Loop)), goto err;)

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_WHILE) goto err;
        parser->data_pos++;

        expr_add
        check_call(scopes_expr(parser, expr_next), goto err;)

        result = SN_Status_Success;
    }
analyze_end
}

int class_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_CLASS) goto end;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_Class;

        expr_add
        check_call(ident_new_expr(parser, expr_next), goto err;)

        parser_get
        if (token->type == TokenType_Special && token->sub_type == Special_LSB) {
            expr_add
            check_call(extends_list(parser, expr_next), goto err;)
        }

        expr_add
        check_call(suite(parser, expr_next, ScopeType_Class), goto err;)
        result = SN_Status_Success;
    }
analyze_end
}
int try_with_stmt(struct parser_st *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_TRY) goto end;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_TryWith;

        expr_add
        check_call(suite(parser, expr_next, parser->scope_type), goto err;)


        parser_end goto eof;
        parser_get
        if (token->type != TokenType_KeyWords || token->sub_type != KeyWord_WITH) goto end;
        parser->data_pos++;

        expr_add
        check_call(suite(parser, expr_next, ScopeType_Class), goto err;)
        result = SN_Status_Success;
    }
analyze_end
}

int compound_stmt(struct parser_st *parser, struct node_st *expr) {
    int result = function_stmt(parser, expr);
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
    if (result != SN_Status_Nothing) return result;
    result = try_with_stmt(parser, expr);
    return result;
}

int statement(struct parser_st *parser, struct node_st *expr) {
    int res = compound_stmt(parser, expr);
    if (res != SN_Status_Nothing) return res;
    analyze_start
    {
        sub_result = stmt_list(parser, expr);
        if (sub_result != SN_Status_Nothing && sub_result != SN_Status_Success) goto sub;

        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || token->sub_type != Special_SEMI) {
            if (sub_result == SN_Status_Nothing) goto end;
            else goto err;
        }
        parser->data_pos++;
        result = SN_Status_Success;
    }
analyze_end
}

int suite(struct parser_st *parser, struct node_st *expr, int scope_type) {
    int res = statement(parser, expr);
    if (res != SN_Status_Nothing) return res;
    analyze_start
    int _scope_type = parser->scope_type;
    parser->scope_type = scope_type;
    size_t variable_count = parser->variables.size;
    size_t variable_stack_size = parser->variables_stack.size;
    size_t closure_stack_size = parser->closures_stack.size;
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || token->sub_type != Special_LCB) goto end;
        parser->data_pos++;

        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_List;
        {
            variable_list_add_new(&parser->variables);
            expr->variable = variable_list_last(&parser->variables);
            variable_list_append(&parser->variables_stack, expr->variable);
            closure_list_append(&parser->closures_stack, expr->closure);
        }

        parser_end goto eof;
        parser_get
        if (token->type == TokenType_Special && token->sub_type == Special_RCB) {
            parser->data_pos++;
            result = SN_Status_Success;
            goto end;
        }

        while (parser->data_pos < parser->tokens.size) {
            expr_add
            parser_get
            if (token->type == TokenType_Special && token->sub_type == Special_LCB)
                check_call(suite(parser, expr_next, parser->scope_type), goto err;)
            else check_call(statement(parser, expr_next), goto err;)

            parser_end goto eof;
            parser_get
            if (token->type == TokenType_Special && token->sub_type == Special_RCB) {
                parser->data_pos++;
                result = SN_Status_Success;
                goto end;
            }
        }
        goto err;
    }
    end:
    parser->scope_type = _scope_type;
    variable_list_resize(&parser->variables_stack, variable_stack_size);
    closure_list_resize(&parser->closures_stack, closure_stack_size);
    if (result != SN_Status_Success) {
        node_clear(expr);
        node_list_resize(&parser->nodes, nodes_count);
        variable_list_resize(&parser->variables, variable_count);
        parser->data_pos = current_pointing;
    }
    return result;
analyze_end_sub
}

int token_analyzer(struct parser_st *parser, struct node_st *expr) {
    parser->data_pos = 0;
    analyze_start
    size_t variable_count = parser->variables.size;
    {
        expr->type = MainType_Stmt;
        expr->sub_type = StmtType_List;
        {
            variable_list_add_new(&parser->variables);
            expr->variable = variable_list_last(&parser->variables);
            variable_list_append(&parser->variables_stack, expr->variable);
            closure_list_append(&parser->closures_stack, NULL);
        }

        while (parser->data_pos < parser->tokens.size) {
            expr_add
            check_call(suite(parser, expr_next, ScopeType_None), goto end;)
        }
        result = SN_Status_Success;
    }
    end:
    variable_list_resize(&parser->variables_stack, 0);
    closure_list_resize(&parser->closures_stack, 0);
    if (result != SN_Status_Success) {
        node_clear(expr);
        node_list_resize(&parser->nodes, nodes_count);
        variable_list_resize(&parser->variables, variable_count);
        parser->data_pos = current_pointing;
    }
    return result;
analyze_end_sub
}

#pragma clang diagnostic pop