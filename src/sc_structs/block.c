#include "sc_structs.h"

struct block_expr_st *block_expr_new() {
    struct block_expr_st *res = skr_malloc(sizeof (struct block_expr_st));
    res->type = BlockType_None;
    res->sub_type = 0;

    res->node = NULL;
    res->code = NULL;

    block_list_data_init(&res->blocks);

    return res;
}
void block_expr_set(struct block_expr_st *res, const struct block_expr_st *a) {
    res->type = a->type;
    res->sub_type = a->sub_type;

    res->node = a->node;
    if(res->code != NULL) bytecode_free(res->code);
    if(a->code != NULL) {
        res->code = bytecode_new();
        bytecode_set(res->code, a->code);
    } else res->code = NULL;

    block_list_set(&res->blocks, &a->blocks);
}
void block_expr_clear(struct block_expr_st *res) {
    res->type = BlockType_None;
    res->sub_type = 0;

    res->node = NULL;

    if(res->code != NULL) bytecode_free(res->code);
    res->code = NULL;

    block_list_clear(&res->blocks);
}
void block_expr_free(struct block_expr_st *res) {
    block_list_data_free(&res->blocks);
    if(res->code != NULL) bytecode_free(res->code);
    skr_free(res);
}

void block_expr_data_init(struct block_expr_st *res) {
    res->type = BlockType_None;
    res->sub_type = 0;

    res->node = NULL;
    res->code = NULL;

    block_list_data_init(&res->blocks);
}
void block_expr_data_free(struct block_expr_st *res) {
    block_list_data_free(&res->blocks);
    if(res->code != NULL) bytecode_free(res->code);
}