#ifndef PARSER_H
#define PARSER_H

#include "struct.h"
#include "token_list.h"
#include "node_list.h"
#include "closure_list.h"

#define MaxBracketNesting 256

struct parser_st{
    char * data_str;
    size_t data_size;
    size_t data_pos;

    size_t line_num;
    size_t line_pos;

    int   scope_buf[MaxBracketNesting];
    size_t scope_pos;

    struct string_st error_msg;
    size_t error_pos;


    struct token_list_st tokens;
    struct node_list_st nodes;
    struct closure_list_st closures;
};

void parser_clear(struct parser_st *);

void parser_data_inti(struct parser_st *);
void parser_data_free(struct parser_st *);

void parser_set_file(struct parser_st *, char *);
void parser_set_str(struct parser_st *, char *, size_t);

#endif //PARSER_H
