#include "sc_structs.h"
#ifdef WIN32
#include <stdio.h>
#endif


struct sc_parser *sc_parser_new(){
    struct sc_parser *res = skr_malloc(sizeof(struct sc_parser));
    res->data = NULL;
    res->size = res->pos = 0;

    res->line_num = res->line_pos = 0;

    res->scope_pos = 0;

    res->list = list_new();

    res->error_msg = string_new();
    res->error_pos = 0;
    return res;
}
void sc_parser_set(struct sc_parser *res, const struct sc_parser *a){
    sc_parser_clear(res);
    res->size = a->size;
    res->data = skr_malloc(res->size);
    memcpy(res->data, a->data, a->size);
}
void sc_parser_clear(struct sc_parser *res){
    if(res->data != NULL) skr_free(res->data);
    res->data = NULL;
    res->size = res->pos = 0;

    res->line_num = res->line_pos = 0;

    res->scope_pos = 0;

    list_clear(res->list);

    string_clear(res->error_msg);
    res->error_pos = 0;
}
void sc_parser_free(struct sc_parser *res){
    if(res->data != NULL) skr_free(res->data);
    list_free(res->list);
    string_free(res->error_msg);
    skr_free(res);
}

void sc_parser_set_str(struct sc_parser *res, char *data, size_t size){
    sc_parser_clear(res);
    res->size = size;
    res->data = skr_malloc(res->size);
    memcpy(res->data, data, size);
}
void sc_parser_set_file(struct sc_parser *res, char *file_path){
    sc_parser_clear(res);

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) return;
    fseek(fp, 0, SEEK_END);
    res->size = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    res->data = skr_malloc(res->size);
#ifdef WIN32
    char c;
    size_t i;
    for (i = 0; i < res->size && (c = (char)getc(fp)) != EOF; i++) {
        res->data[i] = c;
    }
    res->size = i;
#else
    for (size_t i = 0; i < res->size; i++) {
        res->data[i] = (char) getc(fp);
    }
#endif
    fclose(fp);
}