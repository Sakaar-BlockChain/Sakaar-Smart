#include "sc_structs.h"

struct object_type op_func_type = {OP_FUNC_OP, NULL, NULL};

struct op_func *op_func_new(){
    struct op_func *res = skr_malloc(OP_FUNC_SIZE);
    res->args = NULL;
    res->body = NULL;
    return res;
}
void op_func_set(struct op_func *res, const struct op_func *a){
    op_func_clear(res);
    res->args = object_copy(a->args);
    res->body = object_copy(a->body);
}
void op_func_clear(struct op_func *res){
    if(res->args != NULL) object_free(res->args);
    if(res->body != NULL) object_free(res->body);
    res->args = NULL;
    res->body = NULL;
}
void op_func_free(struct op_func *res){
    if(res->args != NULL) object_free(res->args);
    if(res->body != NULL) object_free(res->body);
    skr_free(res);
}