#ifndef AN_PARSER_H
#define AN_PARSER_H

#include "struct.h"
#include "tk_parser.h"

#define MaxBracketNesting 256

struct an_parser{
    size_t position;
    struct list_st *list;
};

struct an_parser *an_parser_new();
void an_parser_set(struct an_parser *, const struct an_parser *);
void an_parser_clear(struct an_parser *);
void an_parser_free(struct an_parser *);

void an_parser_set_list(struct an_parser *, struct tk_parser *);


#endif //AN_PARSER_H
