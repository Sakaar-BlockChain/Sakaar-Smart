#include "sc_structs.h"

struct object_type an_node_type = {AN_NODE_OP, NULL, NULL};

struct an_node *an_node_new(){
    struct an_node *res = skr_malloc(AN_NODE_SIZE);
    res->main_type = MainType_None;
    res->type = ExprType_None;

    res->data = NULL;
    res->next = list_new();
    res->tokens = list_new();
    return res;
}
void an_node_set(struct an_node *res, const struct an_node *a){
    res->main_type = a->main_type;
    res->type = a->type;
    if(res->data != NULL) object_free(res->data);
    res->data = object_copy(a->data);
    list_set(res->next, a->next);
    list_set(res->tokens, a->tokens);
}
void an_node_clear(struct an_node *res){
    res->main_type = MainType_None;
    res->type = ExprType_None;
    if(res->data != NULL) object_free(res->data);
    res->data = NULL;
    list_clear(res->next);
    list_clear(res->tokens);
}
void an_node_free(struct an_node *res){
    if(res->data != NULL) object_free(res->data);
    list_free(res->next);
    list_free(res->tokens);
    skr_free(res);
}