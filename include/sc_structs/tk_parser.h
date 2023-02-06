#ifndef TK_PARSER_H
#define TK_PARSER_H

#include "struct.h"

#define MaxBracketNesting 256

struct tk_parser{
    char *data;
    size_t position;
    size_t str_size;

    size_t current_line;

    char scope_buf[MaxBracketNesting];
    size_t scope_pos;

    struct list_st *list;
};

struct tk_parser *tk_parser_new();
void tk_parser_set(struct tk_parser *, const struct tk_parser *);
void tk_parser_clear(struct tk_parser *);
void tk_parser_free(struct tk_parser *);

void tk_parser_set_str(struct tk_parser *, char *, size_t);
void tk_parser_set_file(struct tk_parser *, char *);

#endif //TK_PARSER_H
