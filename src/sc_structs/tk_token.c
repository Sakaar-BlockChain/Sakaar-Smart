#include "sc_structs.h"

struct object_type tk_token_type = {TK_TOKEN_OP, NULL, NULL};

struct tk_token *tk_token_new(){
    struct tk_token *res = skr_malloc(TK_TOKEN_SIZE);
    res->type = TokenType_None;
    res->subtype = TokenType_None;

    res->data = NULL;
    res->max_size = res->size = 0;
    return res;
}
void tk_token_set(struct tk_token *res, const struct tk_token *a){
    res->type = a->type;
    res->subtype = a->subtype;

    tk_token_resize(res, a->size);
    memcpy(res->data, a->data, a->size);
}
void tk_token_clear(struct tk_token *res){
    res->type = TokenType_None;
    res->subtype = TokenType_None;

    tk_token_resize(res, 0);
}
void tk_token_free(struct tk_token *res){
    if (res->data != NULL) skr_free(res->data);
    skr_free(res);
}

void tk_token_resize(struct tk_token *res, size_t size) {
    if (res->data == NULL && size != 0) {
        res->max_size = size;
        res->data = skr_malloc(size + 1);
        if(res->data != NULL) for (size_t i = 0; i < size + 1; i++) res->data[i] = 0;
    } else if (res->max_size < size) {
        res->data = skr_realloc(res->data, size * 2 + 1);
        if(res->data != NULL) for (size_t i = res->max_size; i < size * 2 + 1; i++) res->data[i] = 0;
        res->max_size = size * 2;
    }
    if (res->size > size) {
        for (size_t i = size; i < res->size; i++) {
            res->data[i] = 0;
        }
    }
    res->size = size;
}