#include "sc_structs.h"

struct node_st *node_new() {
    struct node_st *res = skr_malloc(sizeof(struct node_st));
    res->type = MainType_None;
    res->sub_type = ExprType_None;

    token_list_data_init(&res->tokens);
    node_list_data_init(&res->nodes);

    res->data = NULL;
    res->attrib = NULL;
    res->variable = NULL;
    res->closure = NULL;
    return res;
}
void node_set(struct node_st *res, const struct node_st *a) {
    node_clear(res);

    res->type = a->type;
    res->sub_type = a->sub_type;

    token_list_set(&res->tokens, &a->tokens);
    node_list_set(&res->nodes, &a->nodes);

    if (a->data != NULL) res->data = object_copy(a->data);
    if (a->attrib != NULL) res->attrib = attrib_copy(a->attrib);
    res->variable = a->variable;
    res->closure = a->closure;
}
void node_clear(struct node_st *res) {
    res->type = MainType_None;
    res->sub_type = ExprType_None;

    token_list_clear(&res->tokens);
    node_list_clear(&res->nodes);

    if(res->data != NULL) object_free(res->data);
    if(res->attrib != NULL) attrib_free(res->attrib);
    if(res->attrib != NULL) attrib_free(res->attrib);
    res->data = NULL;
    res->attrib = NULL;
    res->variable = NULL;
    res->closure = NULL;
}
void node_free(struct node_st *res) {
    token_list_data_free(&res->tokens);
    node_list_data_free(&res->nodes);

    if(res->data != NULL) object_free(res->data);
    if(res->attrib != NULL) attrib_free(res->attrib);
    skr_free(res);
}

void node_data_init(struct node_st *res) {
    res->type = MainType_None;
    res->sub_type = ExprType_None;

    token_list_data_init(&res->tokens);
    node_list_data_init(&res->nodes);

    res->data = NULL;
    res->attrib = NULL;
    res->variable = NULL;
    res->closure = NULL;
}
void node_data_free(struct node_st *res) {
    token_list_data_free(&res->tokens);
    node_list_data_free(&res->nodes);

    if(res->data != NULL) object_free(res->data);
    if(res->attrib != NULL) attrib_free(res->attrib);
}