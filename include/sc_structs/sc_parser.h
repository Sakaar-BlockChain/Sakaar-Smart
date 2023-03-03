#ifndef SC_PARSER_H
#define SC_PARSER_H

#include "struct.h"

#define MaxBracketNesting 256

struct sc_parser{
    char *data;
    size_t position;
    size_t str_size;

    size_t current_line;
    size_t line_pos;

    char scope_buf[MaxBracketNesting];
    size_t scope_pos;

    struct list_st *list;

    struct string_st *error_msg;
};

struct sc_parser *sc_parser_new();
void sc_parser_set(struct sc_parser *, const struct sc_parser *);
void sc_parser_clear(struct sc_parser *);
void sc_parser_free(struct sc_parser *);

void sc_parser_set_str(struct sc_parser *, char *, size_t);
void sc_parser_set_file(struct sc_parser *, char *);

#endif //SC_PARSER_H
