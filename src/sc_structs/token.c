#include "sc_structs.h"

struct token_st *token_new(){
    struct token_st *res = skr_malloc(sizeof(struct token_st));
    res->type = TokenType_None;
    res->sub_type = TokenType_None;

    string_data_init(&res->data);
    
    res->line_num = res->line_pos = res->pos = 0;
    return res;
}
void token_set(struct token_st *res, const struct token_st *a){
    res->type = a->type;
    res->sub_type = a->sub_type;

    string_set(&res->data, &a->data);

    res->line_num = a->line_num;
    res->line_pos = a->line_pos;
    res->pos = a->pos;
}
void token_clear(struct token_st *res){
    res->type = TokenType_None;
    res->sub_type = TokenType_None;

    string_clear(&res->data);

    res->line_num = res->line_pos = res->pos = 0;
}
void token_free(struct token_st *res){
    string_data_free(&res->data);
    skr_free(res);
}

void token_data_init(struct token_st *res){
    res->type = TokenType_None;
    res->sub_type = TokenType_None;

    string_data_init(&res->data);

    res->line_num = res->line_pos = res->pos = 0;
}
void token_data_free(struct token_st *res) {
    string_data_free(&res->data);
}

void token_set_pos(struct token_st *res, struct parser_st *parser) {
    res->line_num = parser->line_num;
    res->line_pos = parser->line_pos;
    res->pos = parser->data_pos;
}