#include "sc_structs.h"

struct object_type ast_node_type = {AST_NODE_OP, NULL, NULL};

struct ast_node *ast_node_new(){
    struct ast_node *res = skr_malloc(AST_NODE_SIZE);
    res->main_type = MainType_None;
    res->type = ExprType_None;

    res->memory_pos = 0;
    res->data = NULL;
    res->next = list_new();
    res->tokens = list_new();
    return res;
}
void ast_node_set(struct ast_node *res, const struct ast_node *a){
    res->main_type = a->main_type;
    res->type = a->type;
    if(res->data != NULL) object_free(res->data);
    res->data = object_copy(a->data);
    list_set(res->next, a->next);
    list_set(res->tokens, a->tokens);
}
void ast_node_clear(struct ast_node *res){
    res->main_type = MainType_None;
    res->type = ExprType_None;

    res->memory_pos = 0;
    if(res->data != NULL) object_free(res->data);
    res->data = NULL;
    list_clear(res->next);
    list_clear(res->tokens);
}
void ast_node_free(struct ast_node *res){
    if(res->data != NULL) object_free(res->data);
    list_free(res->next);
    list_free(res->tokens);
    skr_free(res);
}