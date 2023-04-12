#include "sc_structs.h"

struct object_type ast_node_type = {AST_NODE_OP, NULL};

struct ast_node *ast_node_new(){
    struct ast_node *res = skr_malloc(AST_NODE_SIZE);
    res->main_type = MainType_None;
    res->type = ExprType_None;

    res->data = NULL;
    res->closure = NULL;

    res->next = list_new();
    res->tokens = list_new();
    return res;
}
void ast_node_set(struct ast_node *res, const struct ast_node *a){
    res->main_type = a->main_type;
    res->type = a->type;

    if(res->data != NULL) object_free(res->data);
    if(res->closure != NULL) object_free(res->closure);
    res->data = object_copy(a->data);
    res->closure = object_copy(a->closure);

    list_set(res->next, a->next);
    list_set(res->tokens, a->tokens);
}
void ast_node_clear(struct ast_node *res){
    res->main_type = MainType_None;
    res->type = ExprType_None;

    if(res->data != NULL) object_free(res->data);
    if(res->closure != NULL) object_free(res->closure);
    res->data = NULL;
    res->closure = NULL;

    list_clear(res->next);
    list_clear(res->tokens);
}
void ast_node_free(struct ast_node *res){
    if(res->data != NULL) object_free(res->data);
    if(res->closure != NULL) object_free(res->closure);

    list_free(res->next);
    list_free(res->tokens);
    skr_free(res);
}