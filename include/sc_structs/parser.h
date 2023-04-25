#ifndef PARSER_H
#define PARSER_H

#include "struct.h"
#include "closure_list.h"
#include "node_list.h"
#include "token_list.h"
#include "variable_list_list.h"

#define ScopeType_None      0x00
#define ScopeType_Func      0x01
#define ScopeType_Class     0x02
#define ScopeType_Loop      0x04
#define ScopeType_Try       0x05

#define MaxBracketNesting 256

#define Interrupt_None      0x00
#define Interrupt_Continue  0x01
#define Interrupt_Break     0x02
#define Interrupt_Return    0x04
#define Interrupt_Throw     0x08
#define Interrupt_Break_Out 0x09

struct parser_st{
    char * data_str;
    size_t data_size;
    size_t data_pos;

    size_t line_num;
    size_t line_pos;

    int   scope_buf[MaxBracketNesting];
    size_t scope_pos;
    int scope_type;

    struct string_st error_msg;
    size_t error_pos;

    struct closure_list_st closures;
    struct node_list_st nodes;
    struct token_list_st tokens;
    struct variable_list_list_st variables;

    struct closure_list_st closures_stack;
    struct variable_list_list_st variables_stack;
};

void parser_clear(struct parser_st *);

void parser_data_inti(struct parser_st *);
void parser_data_free(struct parser_st *);

void parser_set_file(struct parser_st *, char *);
void parser_set_str(struct parser_st *, char *, size_t);

size_t parser_new_ident(struct parser_st *, struct string_st *);
size_t parser_get_ident(struct parser_st *, struct string_st *);

#endif //PARSER_H
