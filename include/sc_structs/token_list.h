#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include "struct.h"

struct token_list_st{
    struct token_st** tokens;
    size_t size;
    size_t max_size;
    int type;
};

void token_list_set(struct token_list_st *, const struct token_list_st *);
void token_list_clear(struct token_list_st *);

void token_list_data_init(struct token_list_st *);
void token_list_data_free(struct token_list_st *);

void token_list_resize(struct token_list_st *, size_t);
void token_list_append(struct token_list_st *, struct token_st *);
void token_list_add_new(struct token_list_st *);
struct token_st *token_list_last(struct token_list_st *);


void print_token_list(const struct token_list_st *, int);


#endif //TOKEN_LIST_H
