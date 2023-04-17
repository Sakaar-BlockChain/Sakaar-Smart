//#include "sn_syntax.h"
//
//#define expr_cast { \
//struct object_st *obj = object_new(); \
//object_set_type(obj, AST_NODE_TYPE);  \
//ast_node_set(obj->data, expr);        \
//ast_node_clear(expr);                 \
//list_append((expr)->next, obj);       \
//object_free(obj);   \
//}
//#define expr_add { \
//node_list_add_new(&parser->nodes);          \
//expr_next = node_list_last(&parser->nodes); \
//node_list_append(&expr->nodes, expr_next);  \
//}
//
//#define parser_end if (parser->tokens.size <= parser->data_pos)
//#define parser_get token = parser->tokens.tokens[parser->data_pos];
//
//#define analyze_start \
//size_t current_pointing = parser->data_pos; \
//struct node_st *expr_next = expr;           \
//struct token_st *token = NULL;              \
//int result = SN_Status_Nothing, sub_result;
//#define analyze_end \
//end:    if (result != SN_Status_Success) {node_clear(expr);parser->data_pos = current_pointing;} return result; \
//sub:    result = sub_result; goto end; \
//eof:    result = SN_Status_EOF; parser->error_pos = parser->data_pos; goto end; \
//err:    result = SN_Status_Error; parser->error_pos = parser->data_pos; goto end;
//
//#define check_call(call, check) {sub_result = call; if (sub_result == SN_Status_Nothing) check if (sub_result != SN_Status_Success) goto sub;}
//
//int u_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        while (parser->data_pos < parser->tokens.size) {
//            parser_get
//            if (token->type != TokenType_Special || (
//                    token->sub_type != Special_ADD && token->sub_type != Special_SUB))
//                break;
//            parser->data_pos++;
//
//            if (token->sub_type == Special_ADD) continue;
//            token_list_append(&expr->tokens, token);
//        }
//        if (expr->tokens.size % 2 != 0) {
//            expr_add
//            expr->sub_type = ExprType_U;
//            expr->type = MainType_Oper;
//        } else ast_node_clear(expr);
//        check_call(primary_expr(parser, expr_next), {
//            if (expr_next == expr) goto end;
//            else goto err;
//        })
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//int m_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        int times = 0;
//        while (parser->data_pos < parser->tokens.size) {
//            check_call(u_oper(parser, expr_next), {
//                if (times == 0) goto end;
//                else goto err;
//            })
//
//            parser_end break;
//            parser_get
//            if (token->type != TokenType_Special || (
//                    token->sub_type != Special_MUL && token->sub_type != Special_DIV &&
//                    token->sub_type != Special_MOD))
//                break;
//            parser->data_pos++;
//
//            parser_end goto eof;
//            if (times == 0) {
//                expr_cast
//                expr->sub_type = ExprType_M;
//                expr->type = MainType_Oper;
//                times = 1;
//            }
//            expr_add
//            token_list_append(&expr->tokens, token);
//        }
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//int a_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        int times = 0;
//        while (parser->data_pos < parser->tokens.size) {
//            check_call(m_oper(parser, expr_next), {
//                if (times == 0) goto end;
//                else goto err;
//            })
//
//            parser_end break;
//            parser_get
//            if (token->type != TokenType_Special ||
//                (token->sub_type != Special_ADD && token->sub_type != Special_SUB))
//                break;
//            parser->data_pos++;
//
//            parser_end goto eof;
//            if (times == 0) {
//                expr_cast
//                expr->sub_type = ExprType_A;
//                expr->type = MainType_Oper;
//                times = 1;
//            }
//            expr_add
//            token_list_append(&expr->tokens, token);
//        }
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//int shift_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        int times = 0;
//        while (parser->data_pos < parser->tokens.size) {
//            check_call(a_oper(parser, expr_next), {
//                if (times == 0) goto end;
//                else goto err;
//            })
//
//            parser_end break;
//            parser_get
//            if (token->type != TokenType_Special ||
//                (token->sub_type != Special_LSHIFT && token->sub_type != Special_RSHIFT))
//                break;
//            parser->data_pos++;
//
//            parser_end goto eof;
//            if (times == 0) {
//                expr_cast
//                expr->sub_type = ExprType_Shift;
//                expr->type = MainType_Oper;
//                times = 1;
//            }
//            expr_add
//            token_list_append(&expr->tokens, token);
//        }
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//int and_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        int times = 0;
//        while (parser->data_pos < parser->tokens.size) {
//            check_call(shift_oper(parser, expr_next), {
//                if (times == 0) goto end;
//                else goto err;
//            })
//
//            parser_end break;
//            parser_get
//            if (token->type != TokenType_Special || token->sub_type != Special_AND) break;
//            parser->data_pos++;
//
//            parser_end goto eof;
//            if (times == 0) {
//                expr_cast
//                expr->sub_type = ExprType_And;
//                expr->type = MainType_Oper;
//                times = 1;
//            }
//            expr_add
//            token_list_append(&expr->tokens, token);
//        }
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//int xor_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        int times = 0;
//        while (parser->data_pos < parser->tokens.size) {
//            check_call(and_oper(parser, expr_next), {
//                if (times == 0) goto end;
//                else goto err;
//            })
//
//            parser_end break;
//            parser_get
//            if (token->type != TokenType_Special || token->sub_type != Special_XOR) break;
//            parser->data_pos++;
//
//            parser_end goto eof;
//            if (times == 0) {
//                expr_cast
//                expr->sub_type = ExprType_Xor;
//                expr->type = MainType_Oper;
//                times = 1;
//            }
//            expr_add
//            token_list_append(&expr->tokens, token);
//        }
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//int or_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        int times = 0;
//        while (parser->data_pos < parser->tokens.size) {
//            check_call(xor_oper(parser, expr_next), {
//                if (times == 0) goto end;
//                else goto err;
//            })
//
//            parser_end break;
//            parser_get
//            if (token->type != TokenType_Special || token->sub_type != Special_OR) break;
//            parser->data_pos++;
//
//            parser_end goto eof;
//            if (times == 0) {
//                expr_cast
//                expr->sub_type = ExprType_Or;
//                expr->type = MainType_Oper;
//                times = 1;
//            }
//            expr_add
//            token_list_append(&expr->tokens, token);
//        }
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//
//int comp_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        int times = 0;
//        while (parser->data_pos < parser->tokens.size) {
//            check_call(or_oper(parser, expr_next), {
//                if (times == 0) goto end;
//                else goto err;
//            })
//
//            parser_end break;
//            parser_get
//            if (token->type != TokenType_Special || (
//                    token->sub_type != Special_EQ_NOT && token->sub_type != Special_EQ_EQ &&
//                    token->sub_type != Special_EQ_LESS && token->sub_type != Special_EQ_GREATER &&
//                    token->sub_type != Special_LESS && token->sub_type != Special_GREATER))
//                break;
//            parser->data_pos++;
//
//            parser_end goto eof;
//            if (times == 0) {
//                expr_cast
//                expr->sub_type = ExprType_Comp;
//                expr->type = MainType_Oper;
//                times = 1;
//            }
//            expr_add
//            token_list_append(&expr->tokens, token);
//        }
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//int not_test_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        while (parser->data_pos < parser->tokens.size) {
//            parser_get
//            if (token->type != TokenType_Special || token->sub_type != Special_NOT) break;
//            parser->data_pos++;
//
//            token_list_append(&expr->tokens, token);
//        }
//        if (expr->tokens.size % 2 != 0) {
//            expr_add
//            expr->sub_type = ExprType_NotTest;
//            expr->type = MainType_Oper;
//        } else ast_node_clear(expr);
//        check_call(comp_oper(parser, expr_next), {
//            if (expr_next == expr) goto end;
//            else goto err;
//        })
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//int and_test_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        int times = 0;
//        while (parser->data_pos < parser->tokens.size) {
//            check_call(not_test_oper(parser, expr_next), {
//                if (times == 0) goto end;
//                else goto err;
//            })
//
//            parser_end break;
//            parser_get
//            if (token->type != TokenType_Special || token->sub_type != Special_AND_AND) break;
//            parser->data_pos++;
//
//            parser_end goto eof;
//            if (times == 0) {
//                expr_cast
//                expr->sub_type = ExprType_AndTest;
//                expr->type = MainType_Oper;
//                times = 1;
//            }
//            expr_add
//            token_list_append(&expr->tokens, token);
//        }
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//int or_test_oper(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        int times = 0;
//        while (parser->data_pos < parser->tokens.size) {
//            check_call(and_test_oper(parser, expr_next), {
//                if (times == 0) goto end;
//                else goto err;
//            })
//
//            parser_end break;
//            parser_get
//            if (token->type != TokenType_Special || token->sub_type != Special_OR_OR) break;
//            parser->data_pos++;
//
//            parser_end goto eof;
//            if (times == 0) {
//                expr_cast
//                expr->sub_type = ExprType_OrTest;
//                expr->type = MainType_Oper;
//                times = 1;
//            }
//            expr_add
//            token_list_append(&expr->tokens, token);
//        }
//        result = SN_Status_Success;
//    }
//analyze_end
//}
//int list_oper(struct parser_st *parser, struct node_st *expr, short start, short end) {
//    analyze_start
//    {
//        parser_end goto eof;
//        token = parser->list->data[parser->data_pos]->data;
//        if (token->type != TokenType_Special || token->sub_type != start) goto end;
//        parser->data_pos++;
//
//        expr->type = MainType_Expr;
//        expr->sub_type = PrimType_List;
//
//        parser_end goto eof;
//        token = parser->list->data[parser->data_pos]->data;
//        if (token->type == TokenType_Special && token->sub_type == end) {
//            parser->data_pos++;
//            result = SN_Status_Success;
//            goto end;
//        }
//
//        while (parser->data_pos < parser->tokens.size) {
//            expr_add
//            check_call(or_test_oper(parser, expr_next), goto err;)
//
//            parser_end goto eof;
//            parser_get
//            if (token->type == TokenType_Special && token->sub_type == end) {
//                parser->data_pos++;
//                result = SN_Status_Success;
//                goto end;
//            }
//            if (token->type != TokenType_Special || token->sub_type != Special_COMMA) break;
//            parser->data_pos++;
//        }
//        goto err;
//    }
//analyze_end
//}
//
//int list_ident(struct parser_st *parser, struct node_st *expr) {
//    analyze_start
//    {
//        parser_end goto eof;
//        token = parser->list->data[parser->data_pos]->data;
//        if (token->type != TokenType_Special || token->sub_type != Special_LSB) goto end;
//        parser->data_pos++;
//
//        expr->type = MainType_Expr;
//        expr->sub_type = PrimType_List;
//
//        parser_end goto eof;
//        token = parser->list->data[parser->data_pos]->data;
//        if (token->type == TokenType_Special && token->sub_type == Special_RSB) {
//            parser->data_pos++;
//            result = SN_Status_Success;
//            goto end;
//        }
//
//        while (parser->data_pos < parser->tokens.size) {
//            expr_add
//            check_call(ident_new_expr(parser, expr_next), goto err;)
//
//            parser_end goto eof;
//            parser_get
//            if (token->type == TokenType_Special && token->sub_type == Special_RSB) {
//                parser->data_pos++;
//                result = SN_Status_Success;
//                goto end;
//            }
//            if (token->type != TokenType_Special || token->sub_type != Special_COMMA) break;
//            parser->data_pos++;
//        }
//        goto err;
//    }
//analyze_end
//}