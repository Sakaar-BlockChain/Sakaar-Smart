#include <stdio.h>
#include <time.h>
#include "op_operations.h"
#include "cg_code.h"
#include "sys/time.h"

#define PRINT_PREF for(int _i=0;_i<size;_i++)printf("%c",prefix[_i]);
#define PRINT_NEXT(expr) if (expr) {printf("\t├- ");prefix[size + 1] = '|';}else{printf("\t└- ");prefix[size + 1] = ' ';}prefix[size] = '\t';

char prefix[100];
void *printing[100];
size_t printing_pos;

void print_op_function(const struct op_function *res, int size);

void print_int(const struct integer_st *res) {
    printf("integer : ");
#ifdef USE_GMP
    gmp_printf("%Zd", res->mpz_int);
#else
    struct string_st str;
    string_data_init(&str);

    integer_get_str(res, &str);
    for(int i=0;i<str.size;i++)printf("%c", str.data[i]);
    string_data_free(&str);
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
void print_list(const struct list_st *res, int size) {
    printf("list (%zu):\n", res->size);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_obj(res->data[i], size + 2);
    }
}
void print_obj(const struct object_st *res, int size) {
    if (res == NULL) {
        printf("None\n");
        return;
    }
    printf("object : (%zu) %p\n", res->counter, res);
    PRINT_PREF
    PRINT_NEXT(0)
    for(size_t i=0;i<printing_pos;i++) {
        if (printing[i] == res) {
            printf("...\n");
            return;
        }
    }
    printing[printing_pos++] = (void *) res;
    if (res->type == NONE_TYPE) printf("None\n");
    else if (res->type == STRING_TYPE) print_str(res->data);
    else if (res->type == TLV_TYPE) print_tlv(res->data);
    else if (res->type == INTEGER_TYPE) print_int(res->data);
    else if (res->type == OBJECT_TYPE) print_obj(res->data, size + 2);
    else if (res->type == LIST_TYPE) print_list(res->data, size + 2);
    else if (res->type == OP_CLASS_TYPE) printf("OP_CLASS\n");
    else if (res->type == OP_FUNCTION_TYPE) print_op_function(res->data, size + 2);
    else if (res->type == OP_OBJECT_TYPE) printf("OP_OBJECT\n");
    printing[--printing_pos] = NULL;
}

void print_code(char command, size_t data) {
    switch (command) {
        case BC_Init:
            printf("BC_Init         ");
            break;
        case BC_Load:
            printf("BC_Load         ");
            break;
        case BC_LoadConst:
            printf("BC_LoadConst    ");
            break;
        case BC_Attrib:
            printf("BC_Attrib       ");
            break;
        case BC_Subscript:
            printf("BC_Subscript    ");
            break;
        case BC_Call:
            printf("BC_Call         ");
            break;
        case BC_FuncEnd:
            printf("BC_FuncEnd      ");
            break;
        case BC_ClassEnd:
            printf("BC_ClassEnd     ");
            break;
        case BC_Pop:
            printf("BC_Pop          ");
            break;

        case BC_Convert_Bool:
            printf("BC_Convert_Bool ");
            break;
        case BC_Convert_Int:
            printf("BC_Convert_Int  ");
            break;
        case BC_Convert_Float:
            printf("BC_Convert_Float");
            break;
        case BC_Convert_Str:
            printf("BC_Convert_Str  ");
            break;

        case BC_Compare:
            printf("BC_Compare      ");
            break;
        case BC_Arithmetic:
            printf("BC_Arithmetic   ");
            break;
        case BC_ArithmeticSet:
            printf("BC_ArithmeticSet");
            break;
        case BC_Negate:
            printf("BC_Negate       ");
            break;
        case BC_NegateBool:
            printf("BC_NegateBool   ");
            break;
        case BC_Set:
            printf("BC_Set          ");
            break;

        case BC_Jump:
            printf("BC_Jump         ");
            break;
        case BC_IfTrueOrPop:
            printf("BC_IfTrueOrPop  ");
            break;
        case BC_IfFalseOrPop:
            printf("BC_IfFalseOrPop ");
            break;
        case BC_IfFalse_Jump:
            printf("BC_IfFalse_Jump ");
            break;

        case BC_MakeFunc:
            printf("BC_MakeFunc     ");
            break;
        case BC_MakeClass:
            printf("BC_MakeClass    ");
            break;
        case BC_MakeList:
            printf("BC_MakeList     ");
            break;

        case BC_Break:
            printf("BC_Break        ");
            break;
        case BC_Throw:
            printf("BC_Throw        ");
            break;
        case BC_Return:
            printf("BC_Return       ");
            break;
        case BC_Continue:
            printf("BC_Continue     ");
            break;
        default:
            break;
    }
    printf("\t%zu", data);
}

void print_bytecode(const struct bytecode_st *res, int size) {
    printf("bytecode (%lu) : (%p)\n", res->size, res);
    for(size_t i=0;i<res->size;i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        printf("%.4zx\t", i);
        print_code(res->command[i], res->data[i]);
        printf("\t%p\n", &res->data[i]);
    }
}
void print_bytecode_list(const struct bytecode_list_st *res, int size) {
    printf("bytecode_list (%zu) : (%p)\n", res->size, res);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_bytecode(res->bytecodes[i], size + 2);
    }
}


void print_frame(const struct frame_st *res, int size) {
    printf("frame : \n");
    PRINT_PREF
    PRINT_NEXT(1)
    print_variable_list(&res->attrib, size + 2);
    PRINT_PREF
    PRINT_NEXT(0)
    print_list(&res->data, size + 2);
}

void print_variable(const struct variable_st *res, int size) {
    printf("variable : (%p)\n", res);
    PRINT_PREF
    PRINT_NEXT(1)
    print_str(&res->name);
    PRINT_PREF
    PRINT_NEXT(0)
    printf("position : %zu\n", res->position);
}
void print_variable_list(const struct variable_list_st *res, int size) {
    printf("variable_list (%zu):\n", res->size);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_variable(res->variables[i], size + 2);
    }
}
void print_variable_list_list(const struct variable_list_list_st *res, int size) {
    printf("variable_list_list (%zu):\n", res->size);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_variable_list(res->variable_lists[i], size + 2);
    }
}

//void print_frame(const struct frame_st *res, int size) {
//    printf("frame : (%p)\n", res);
//    PRINT_PREF
//    PRINT_NEXT(1)
//    print_attrib_list(&res->attrib, size + 2);
//    PRINT_PREF
//    PRINT_NEXT(0)
//    print_list(&res->data, size + 2);
//}
//void print_frame_list(const struct frame_list_st *res, int size) {
//    printf("frames (%zu):\n", res->size);
//    for (int i = 0; i < res->size; i++) {
//        PRINT_PREF
//        PRINT_NEXT(i + 1 < res->size)
//        print_frame(res->frames[i], size + 2);
//    }
//}

void print_closure(const struct closure_st *res, int size) {
    printf("closure : (%p)\n", res);
    PRINT_PREF
    PRINT_NEXT(1)
    print_variable_list(&res->attrib, size + 2);
    PRINT_PREF
    PRINT_NEXT(0)
    print_variable_list(&res->data, size + 2);
}
void print_closure_list(const struct closure_list_st *res, int size) {
    printf("closure_list (%zu):\n", res->size);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_closure(res->closures[i], size + 2);
    }
}

void print_token(const struct token_st *res, int size) {
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
        switch (res->sub_type) {
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
        switch (res->sub_type) {
            case KeyWord_DO:
                printf("KeyWord_DO ");
                break;
            case KeyWord_IF:
                printf("KeyWord_IF ");
                break;
            case KeyWord_IN:
                printf("KeyWord_IN ");
                break;
            case KeyWord_TRY:
                printf("KeyWord_TRY ");
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
            case KeyWord_WITH:
                printf("KeyWord_WITH ");
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
            case KeyWord_SWITCH:
                printf("KeyWord_SWITCH ");
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
        for (int i = 0; i < res->data.size; i++) printf("%c", res->data.data[i]);
        printf("\n");
    } else if (res->type == TokenType_Special) {
        PRINT_PREF
        PRINT_NEXT(0)
        switch (res->sub_type) {
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
void print_token_list(const struct token_list_st *res, int size) {
    printf("token_list (%zu):\n", res->size);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_token(res->tokens[i], size + 2);
    }
}

void print_node(const struct node_st *res, int size) {
    printf("Expr : ");
    switch (res->type) {
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
        case MainType_Impr:
            printf("MainType_Impr ");
            break;
    }
    if (res->type == MainType_Expr) {
        switch (res->sub_type) {
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
    if (res->type == MainType_Oper) {
        switch (res->sub_type) {
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
    if (res->type == MainType_Stmt) {
        switch (res->sub_type) {
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
            case StmtType_Oper:
                printf("StmtType_Oper ");
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
            case StmtType_Func_Body:
                printf("StmtType_Func_Body ");
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
            case StmtType_Class_Body:
                printf("StmtType_Class_Body ");
                break;
            case StmtType_List:
                printf("StmtType_List ");
                break;
        }
    }
    if (res->type == MainType_Impr) {
        switch (res->sub_type) {
            case ImprType_Name:
                printf("ImprType_Name ");
                break;
            case ImprType_Module_As:
                printf("ImprType_Module_As ");
                break;
            case ImprType_Module_All:
                printf("ImprType_Module_All ");
                break;
            case ImprType_From:
                printf("ImprType_From ");
                break;
            case ImprType_Import:
                printf("ImprType_Import ");
                break;
        }
    }
    printf("\n");
    if (res->type == MainType_Expr && (res->sub_type == PrimType_Ident_get || res->sub_type == PrimType_Ident_new ||
                                       res->sub_type == PrimType_Attrib || res->sub_type == PrimType_Literal)) {
        PRINT_PREF
        PRINT_NEXT(res->tokens.size != 0 || res->nodes.size != 0 || res->variable != 0 || res->closure != 0)
        printf("data : %zu\n", res->data);
    }
    if (res->tokens.size != 0) {
        PRINT_PREF
        PRINT_NEXT(res->nodes.size != 0 || res->variable != 0 || res->closure != 0)
        print_token_list(&res->tokens, size + 2);
    }
    if (res->nodes.size != 0) {
        PRINT_PREF
        PRINT_NEXT(res->variable != 0 || res->closure != 0)
        print_node_list(&res->nodes, size + 2);
    }
//    if (res->variable != NULL) {
//        PRINT_PREF
//        PRINT_NEXT(res->closure != NULL)
//        print_variable_list(res->variable, size + 2);
//    }
//    if (res->closure != NULL) {
//        PRINT_PREF
//        PRINT_NEXT(0)
//        print_closure(res->closure, size + 2);
//    }
}
void print_node_list(const struct node_list_st *res, int size) {
    printf("node_list (%zu):\n", res->size);
    for (int i = 0; i < res->size; i++) {
        PRINT_PREF
        PRINT_NEXT(i + 1 < res->size)
        print_node(res->nodes[i], size + 2);
    }
}

void print_op_function(const struct op_function *res, int size) {
    printf("OP Function : \n");
    PRINT_PREF
    PRINT_NEXT(1)
    print_frame(res->closure, size + 2);
    PRINT_PREF
    PRINT_NEXT(1)
    printf("argument: %zu\n", res->argument);
    PRINT_PREF
    PRINT_NEXT(1)
    printf("argument_size: %zu\n", res->argument_size);
    PRINT_PREF
    PRINT_NEXT(0)
    printf("function_body: %zu\n", res->function_body);
//    if (res->variable != NULL) {
//        PRINT_PREF
//        PRINT_NEXT(res->closure != NULL)
//        print_variable_list(res->variable, size + 2);
//    }
//    if (res->closure != NULL) {
//        PRINT_PREF
//        PRINT_NEXT(0)
//        print_closure(res->closure, size + 2);
//    }
}


int main(int args, char *argv[]) {
    struct parser_st parser;
    parser_data_init(&parser);

    parser_set_file(&parser, argv[1]); // Open File
    if (parser.error->present) {
        printf("%s: %s\n", parser.error->type.data, parser.error->msg.data);
        parser_data_free(&parser);
        exit(1);
    }

    tokenize(&parser); // Tokenize file
    if (parser.error->present) {
        printf("%s: %s\n", parser.error->type.data, parser.error->msg.data);
        printf("Line %zu: \n", parser.error->line_num + 1);
        for (size_t i = parser.error->line_pos; i < parser.data_size; i++) {
            if (parser.data_str[i] == '\n') break;
            printf("%c", parser.data_str[i]);
        }
        printf("\n");
        for (size_t i = parser.error->line_pos; i < parser.error->pos; i++) printf(" ");
        printf("^\n");

        parser_data_free(&parser);
        exit(1);
    }

    token_analyzer(&parser); // Make simple AST Tree
    if (parser.error->present) {
        printf("%s: %s\n", parser.error->type.data, parser.error->msg.data);
        printf("Line %zu: \n", parser.error->line_num + 1);
        for (size_t i = parser.error->line_pos; i < parser.data_size; i++) {
            if (parser.data_str[i] == '\n') break;
            printf("%c", parser.data_str[i]);
        }
        printf("\n");
        for (size_t i = parser.error->line_pos; i < parser.error->pos; i++) printf(" ");
        printf("^\n");

        parser_data_free(&parser);
        exit(1);
    }
    print_node(parser.nodes.nodes[0], 0);

    cg_generate(&parser);
    print_bytecode_list(&parser.codes, 0);

    clock_t begin = clock();

    sc_first_run(&parser);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    print_variable_list_list(&parser.variables, 0);
    print_closure_list(&parser.closures, 0);

    if (parser.error->present) {
        printf("%s: %s\n", parser.error->type.data, parser.error->msg.data);
        exit(1);
    }
    printf("Time : %f\n", time_spent);

    parser_data_free(&parser);
    memory_clear();
}

// Сделать сериализацию обектов
// У обектов будет две переменые _encode _decode
// работать так же как с обычными обектами

