#include "sc_structs.h"

struct an_parser *an_parser_new(){
    struct an_parser *res = skr_malloc(sizeof(struct an_parser));
    res->position = 0;
    res->list = list_new();
    return res;
}
void an_parser_set(struct an_parser *res, const struct an_parser *a){
    res->position = a->position;
    list_set(res->list, a->list);
}
void an_parser_clear(struct an_parser *res){
    res->position = 0;
    list_clear(res->list);
}
void an_parser_free(struct an_parser *res){
    list_free(res->list);
    skr_free(res);
}

void an_parser_set_list(struct an_parser *res, struct tk_parser *parser){
    res->position = 0;
    list_set(res->list, parser->list);
}