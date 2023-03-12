#include <stdio.h>
#include "sm_semantic.h"

#define PRINT_PREF for(int _i=0;_i<size;_i++)printf("%c",prefix[_i]);
#define PRINT_NEXT(expr) if(expr){printf("\t├- ");prefix[size + 1] = '|';}else{printf("\t└- ");prefix[size + 1] = ' ';}prefix[size] = '\t';

char prefix[100];


void print_int(const struct integer_st *res) {
    printf("integer : ");
#ifdef USE_GMP
    gmp_printf("%Zx", res->mpz_int);
#else
    struct string_st *str = string_new();
    integer_get_str(res, str);
    for(int i=0;i<str->size;i++)printf("%c", str->data[i]);
    string_free(str);
#endif
    printf("\n");
}
void print_str(const struct string_st *res) {
    printf("string (%zu): ", res->size);
    for (int i = 0; i < res->size; i++) printf("%c", res->data[i]);
    printf("\n");
}
void print_tlv(const struct string_st *res) {
    printf("tlv : ");
    for (int i = 0; i < res->size; i++)printf("%.2x ", (unsigned char) res->data[i]);
    printf("\n");
}
void print_token(const struct tk_token *res, int size) {
    printf("Token : ");
    switch (res->type) {
        case TokenType_None:
            printf("TokenType_None ");
            break;
        case TokenType_KeyWords:
            printf("TokenType_KeyWords ");
            break;
        case TokenType_Identifier:
            printf("TokenType_Identifier ");
            break;
        case TokenType_String:
            printf("TokenType_String ");
            break;
        case TokenType_Int:
            printf("TokenType_Int ");
            break;
        case TokenType_Special:
            printf("TokenType_Special ");
            break;
    }
    if (res->type == TokenType_Int) {
        switch (res->subtype) {
            case IntType_bin:
                printf("IntType_bin ");
                break;
            case IntType_oct:
                printf("IntType_oct ");
                break;
            case IntType_dec:
                printf("IntType_dec ");
                break;
            case IntType_hex:
                printf("IntType_hex ");
                break;
            case IntType_float:
                printf("IntType_float ");
                break;
        }
    }
    if (res->type == TokenType_KeyWords) {
        switch (res->subtype) {
            case KeyWord_DO:
                printf("KeyWord_DO ");
                break;
            case KeyWord_IF:
                printf("KeyWord_IF ");
                break;
            case KeyWord_IN:
                printf("KeyWord_IN ");
                break;
            case KeyWord_FOR:
                printf("KeyWord_FOR ");
                break;
            case KeyWord_CASE:
                printf("KeyWord_CASE ");
                break;
            case KeyWord_ELSE:
                printf("KeyWord_ELSE ");
                break;
            case KeyWord_TRUE:
                printf("KeyWord_TRUE ");
                break;
            case KeyWord_FUNC:
                printf("KeyWord_FUNC ");
                break;
            case KeyWord_BREAK:
                printf("KeyWord_BREAK ");
                break;
            case KeyWord_CLASS:
                printf("KeyWord_CLASS ");
                break;
            case KeyWord_WHILE:
                printf("KeyWord_WHILE ");
                break;
            case KeyWord_FALSE:
                printf("KeyWord_FALSE ");
                break;
            case KeyWord_PUBLIC:
                printf("KeyWord_PUBLIC ");
                break;
            case KeyWord_SWITCH:
                printf("KeyWord_SWITCH ");
                break;
            case KeyWord_STATIC:
                printf("KeyWord_STATIC ");
                break;
            case KeyWord_RETURN:
                printf("KeyWord_RETURN ");
                break;
            case KeyWord_CONTINUE:
                printf("KeyWord_CONTINUE ");
                break;
        }
    }
    printf("\n");
    if (res->type == TokenType_Int || res->type == TokenType_Identifier || res->type == TokenType_String) {
        PRINT_PREF
        PRINT_NEXT(0)
        for (int i = 0; i < res->size; i++) printf("%c", res->data[i]);
        printf("\n");
    } else if (res->type == TokenType_Special) {
        PRINT_PREF
        PRINT_NEXT(0)
        switch (res->subtype) {
            case Special_MOD:
                printf("%c", '%');
                break;
            case Special_AND:
                printf("&");
                break;
            case Special_LSB:
                printf("(");
                break;
            case Special_RSB:
                printf(")");
                break;
            case Special_MUL:
                printf("*");
                break;
            case Special_ADD:
                printf("+");
                break;
            case Special_COMMA:
                printf(",");
                break;
            case Special_SUB:
                printf("-");
                break;
            case Special_DOT:
                printf(".");
                break;
            case Special_DIV:
                printf("/");
                break;
            case Special_COLON:
                printf(":");
                break;
            case Special_SEMI:
                printf(";");
                break;
            case Special_LESS:
                printf("<");
                break;
            case Special_EQ:
                printf("=");
                break;
            case Special_GREATER:
                printf(">");
                break;
            case Special_NOT:
                printf("!");
                break;
            case Special_LSQB:
                printf("[");
                break;
            case Special_RSQB:
                printf("]");
                break;
            case Special_XOR:
                printf("^");
                break;
            case Special_LCB:
                printf("{");
                break;
            case Special_RCB:
                printf("}");
                break;
            case Special_OR:
                printf("|");
                break;

            case Special_EQ_MOD:
                printf("%c=", '%');
                break;
            case Special_AND_AND:
                printf("&&");
                break;
            case Special_EQ_AND:
                printf("&=");
                break;
            case Special_EQ_MUL:
                printf("*=");
                break;
            case Special_EQ_ADD:
                printf("+=");
                break;
            case Special_EQ_SUB:
                printf("-=");
                break;
            case Special_EQ_DIV:
                printf("/=");
                break;
            case Special_LSHIFT:
                printf("<<");
                break;
            case Special_EQ_LESS:
                printf("<=");
                break;
            case Special_EQ_EQ:
                printf("==");
                break;
            case Special_RSHIFT:
                printf(">>");
                break;
            case Special_EQ_GREATER:
                printf(">=");
                break;
            case Special_EQ_NOT:
                printf("!=");
                break;
            case Special_EQ_XOR:
                printf("^=");
                break;
            case Special_OR_OR:
                printf("||");
                break;
            case Special_EQ_OR:
                printf("|=");
                break;
            case Special_COM_LINE:
                printf("//");
                break;
            case Special_COM_STR:
                printf("/*");
                break;
            case Special_COM_END:
                printf("*/");
                break;
            case Special_EQ_LSHIFT:
                printf("<<=");
                break;
            case Special_EQ_RSHIFT:
                printf(">>=");
                break;
        }
        printf("\n");
    }
}
void print_node(const struct ast_node *res, int size) {
    printf("Expr : ");
    switch (res->main_type) {
        case MainType_None:
            printf("MainType_None ");
            break;
        case MainType_Expr:
            printf("MainType_Expr ");
            break;
        case MainType_Oper:
            printf("MainType_Oper ");
            break;
        case MainType_Stmt:
            printf("MainType_Stmt ");
            break;
    }
    if (res->main_type == MainType_Expr) {
        switch (res->type) {
            case PrimType_List:
                printf("PrimType_List ");
                break;
            case PrimType_Ident_get:
                printf("PrimType_Ident_get ");
                break;
            case PrimType_Ident_new:
                printf("PrimType_Ident_new ");
                break;
            case PrimType_Literal:
                printf("PrimType_Literal ");
                break;
            case PrimType_Attrib:
                printf("PrimType_Attrib ");
                break;
            case PrimType_Subscript:
                printf("PrimType_Subscript ");
                break;
            case PrimType_Call:
                printf("PrimType_Call ");
                break;
        }
    }
    if (res->main_type == MainType_Oper) {
        switch (res->type) {
            case ExprType_U:
                printf("ExprType_U ");
                break;
            case ExprType_M:
                printf("ExprType_M ");
                break;
            case ExprType_A:
                printf("ExprType_A ");
                break;
            case ExprType_Shift:
                printf("ExprType_Shift ");
                break;
            case ExprType_And:
                printf("ExprType_And ");
                break;
            case ExprType_Xor:
                printf("ExprType_Xor ");
                break;
            case ExprType_Or:
                printf("ExprType_Or ");
                break;
            case ExprType_Comp:
                printf("ExprType_Comp ");
                break;
            case ExprType_NotTest:
                printf("ExprType_NotTest ");
                break;
            case ExprType_AndTest:
                printf("ExprType_AndTest ");
                break;
            case ExprType_OrTest:
                printf("ExprType_OrTest ");
                break;
        }
    }
    if (res->main_type == MainType_Stmt) {
        switch (res->type) {
            case StmtType_Annot:
                printf("StmtType_Annot ");
                break;
            case StmtType_Assign:
                printf("StmtType_Assign ");
                break;
            case StmtType_Return:
                printf("StmtType_Return ");
                break;
            case StmtType_Break:
                printf("StmtType_Break ");
                break;
            case StmtType_Continue:
                printf("StmtType_Continue ");
                break;
            case StmtType_Params:
                printf("StmtType_Params ");
                break;
            case StmtType_Extends:
                printf("StmtType_Extends ");
                break;
            case StmtType_Func:
                printf("StmtType_Func ");
                break;
            case StmtType_PUB_Func:
                printf("StmtType_PUB_Func ");
                break;
            case StmtType_STC_Func:
                printf("StmtType_STC_Func ");
                break;
            case StmtType_PRI_Func:
                printf("StmtType_PRI_Func ");
                break;
            case StmtType_If:
                printf("StmtType_If ");
                break;
            case StmtType_While:
                printf("StmtType_While ");
                break;
            case StmtType_DoWhile:
                printf("StmtType_DoWhile ");
                break;
            case StmtType_Class:
                printf("StmtType_Class ");
                break;
            case StmtType_List:
                printf("StmtType_List ");
                break;
        }
    }
    printf("\n");
    if (res->data != NULL) {
        PRINT_PREF
        PRINT_NEXT(!list_is_null(res->tokens) || !list_is_null(res->next))
        print_obj(res->data, size + 2);
    }
    if (!list_is_null(res->tokens)) {
        PRINT_PREF
        PRINT_NEXT(!list_is_null(res->next))
        print_list(res->tokens, size + 2);
    }
    if (!list_is_null(res->next)) {
        PRINT_PREF
        PRINT_NEXT(0)
        print_list(res->next, size + 2);
    }
}
void print_list(const struct list_st *res, int size) {
    printf("list (%zu):\n", res->size);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_obj(res->data[i], size + 2);
    }
}
void print_stack(const struct stack_st *res, int size) {
    printf("stack (%zu):\n", res->size);
    for (struct stack_el *elm = res->top; elm != NULL; elm = elm->priv) {
        PRINT_PREF
        PRINT_NEXT(elm->priv != NULL)
        print_obj(elm->data, size + 2);
    }
}
//void print_block(const struct op_block *res, int size) {
//    printf("block :");
//    switch (res->type) {
//        case BlockType_None:
//            printf("BlockType_None ");
//            break;
//        case BlockType_Convert:
//            printf("BlockType_Convert ");
//            switch (res->subtype) {
//                case Convert_Bool:
//                    printf("Convert_Bool ");
//                    break;
//                case Convert_Int:
//                    printf("Convert_Int ");
//                    break;
//                case Convert_Float:
//                    printf("Convert_Float ");
//                    break;
//                case Convert_Str:
//                    printf("Convert_Str ");
//                    break;
//            }
//            break;
//        case BlockType_Arithmetic:
//            printf("BlockType_Arithmetic ");
//            break;
//        case BlockType_If_not:
//            printf("BlockType_If_not ");
//            break;
//        case BlockType_If_not_del:
//            printf("BlockType_If_not_del ");
//            break;
//        case BlockType_If:
//            printf("BlockType_If ");
//            break;
//        case BlockType_If_del:
//            printf("BlockType_If_del ");
//            break;
//
//        case BlockType_Put:
//            printf("BlockType_Put ");
//            break;
//        case BlockType_Delete_Temp:
//            printf("BlockType_Delete_Temp ");
//            break;
//        case BlockType_Continue:
//            printf("BlockType_Continue ");
//            break;
//        case BlockType_Break:
//            printf("BlockType_Break ");
//            break;
//
//        case BlockType_List:
//            printf("BlockType_List ");
//            break;
//        case BlockType_Attr:
//            printf("BlockType_Attr ");
//            break;
//        case BlockType_Func:
//            printf("BlockType_Func ");
//            break;
//        case BlockType_Call:
//            printf("BlockType_Call ");
//            break;
//    }
//    printf("%zu\n", res->count);
//    if(res->data1 != NULL){
//        PRINT_PREF
//        PRINT_NEXT(res->data2 != NULL)
//        print_obj(res->data1, size + 2);
//    }
//    if(res->data2 != NULL){
//        PRINT_PREF
//        PRINT_NEXT(0)
//        print_obj(res->data2, size + 2);
//    }
//}
void print_map(const struct map_st *res, int size) {
    printf("Map\n");
}
void print_obj(const struct object_st *res, int size) {
    printf("object : (%d)\n", res->counter);
    PRINT_PREF
    PRINT_NEXT(0)
    if (res->type == NONE_TYPE) printf("None\n");
    else if (res->type == STRING_TYPE) return print_str(res->data);
    else if (res->type == TLV_TYPE) return print_tlv(res->data);
    else if (res->type == INTEGER_TYPE) return print_int(res->data);
    else if (res->type == OBJECT_TYPE) return print_obj(res->data, size + 2);
    else if (res->type == LIST_TYPE) return print_list(res->data, size + 2);
    else if (res->type == TK_TOKEN_TYPE) return print_token(res->data, size + 2);
    else if (res->type == AST_NODE_TYPE) return print_node(res->data, size + 2);
//    else if (res->type == OP_BLOCK_TYPE) return print_block(res->data, size + 2);
    else if (res->type == MAP_TYPE) return print_map(res->data, size + 2);
}

//void run_smart_contract(struct op_state *state);
//void run_an(struct op_state *state, struct ast_node *node);

int main() {
    struct sc_parser *parser = sc_parser_new();
    struct object_st *expr_obj = object_new();
    object_set_type(expr_obj, AST_NODE_TYPE);

    sc_parser_set_file(parser, "text.txt");
    tokenize(parser);
    if (string_is_null(parser->error_msg)) {
//        print_list(parser->list, 0);

        char res = token_analyzer(parser, expr_obj->data);
        printf("Result : %d\n", res);
        if(res != SN_Status_Success){
            printf("Error_pos : %d\n", parser->error_pos);
            struct tk_token *token = parser->list->data[parser->error_pos]->data;
            print_token(token, 0);

            printf("\nLine %zu: \n", token->line_num + 1);
            for (size_t i = token->line_pos; i < parser->size; i++) {
                if (parser->data[i] == '\n') break;
                printf("%c", parser->data[i]);
            }
            printf("\n");
            for (size_t i = token->line_pos; i < token->pos; i++) printf(" ");
            printf("^\n");
        }else{
            print_obj(expr_obj, 0);
        }

        {
            int res1 = scan_node(expr_obj, 0);
            printf("Result : %d\n", res1);
        }
    } else {
        printf("Error : ");
        for (int i = 0; i < parser->error_msg->size; i++) printf("%c", parser->error_msg->data[i]);
        printf("\nLine %zu: \n", parser->line_num + 1);
        for (size_t i = parser->line_pos; i < parser->size; i++) {
            if (parser->data[i] == '\n') break;
            printf("%c", parser->data[i]);
        }
        printf("\n");
        for (size_t i = parser->line_pos; i < parser->pos; i++) printf(" ");
        printf("^\n");
    }

    object_free(expr_obj);
    sc_parser_free(parser);
//    struct op_state *state = op_state_new();
//    //    clock_t begin = clock();
//    //    stmt_run(expr_obj, state);
//    //    clock_t end = clock();
//    //    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//    //    list_print(state->memory_names,0);
//    //    list_print(state->memory,0);
//    //    printf("TIME : %f\n", time_spent);
//
//    stack_add(state->code_operations, expr_obj);
//    stack_add_new(state->memory, MAP_TYPE);
//
//    run_smart_contract(state);
//    printf("\n");
//    print_stack(state->temp_memory, 0);
//    print_stack(state->code_operations, 0);
//    op_state_free(state);

    printf("%zu\n", mem_ctx.filled);
}

// Сделать сериализацию обектов
// У обектов будет две переменые _encode _decode
// работать так же как с обычными обектами

