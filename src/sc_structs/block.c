#include "sc_structs.h"

struct block_expr_st *block_expr_new() {
    struct block_expr_st *res = skr_malloc(sizeof (struct block_expr_st));
    res->type = BlockType_None;
    res->sub_type = 0;
    res->count = 0;

    res->node = NULL;

    res->data = NULL;
    res->attrib = NULL;
    res->variable = NULL;
    res->closure = NULL;
    res->token = NULL;

    block_list_data_init(&res->blocks);

    return res;
}
void block_expr_set(struct block_expr_st *res, const struct block_expr_st *a) {
    res->type = a->type;
    res->sub_type = a->sub_type;
    res->count = a->count;

    res->node = a->node;

    res->data = object_copy(a->data);
    res->attrib = attrib_copy(a->attrib);
    res->variable = a->variable;
    res->closure = a->closure;
    res->token = a->token;

    block_list_set(&res->blocks, &a->blocks);
}
void block_expr_clear(struct block_expr_st *res) {
    res->type = BlockType_None;
    res->sub_type = 0;
    res->count = 0;

    res->node = NULL;

    res->data = NULL;
    res->attrib = NULL;
    res->variable = NULL;
    res->closure = NULL;
    res->token = NULL;

    block_list_clear(&res->blocks);
}
void block_expr_free(struct block_expr_st *res) {
    block_list_data_free(&res->blocks);
    if(res->data != NULL) object_free(res->data);
    if(res->attrib != NULL) attrib_free(res->attrib);
    skr_free(res);
}

void block_expr_data_init(struct block_expr_st *res) {
    res->type = BlockType_None;
    res->sub_type = 0;
    res->count = 0;

    res->node = NULL;

    res->data = NULL;
    res->attrib = NULL;
    res->variable = NULL;
    res->closure = NULL;
    res->token = NULL;

    block_list_data_init(&res->blocks);
}
void block_expr_data_free(struct block_expr_st *res) {
    block_list_data_free(&res->blocks);
    if(res->data != NULL) object_free(res->data);
    if(res->attrib != NULL) attrib_free(res->attrib);
}