#include "sc_structs.h"
#ifdef WIN32
#include <stdio.h>
#endif


struct tk_parser *tk_parser_new(){
    struct tk_parser *res = skr_malloc(sizeof(struct tk_parser));
    res->data = NULL;
    res->position = res->str_size = res->line_pos = 0;

    res->current_line = 0;
    res->scope_pos = 0;
    res->list = list_new();
    res->error_msg = string_new();
    return res;
}
void tk_parser_set(struct tk_parser *res, const struct tk_parser *a){
    tk_parser_clear(res);
    res->str_size = a->str_size;
    res->data = skr_malloc(res->str_size);
    memcpy(res->data, a->data, a->str_size);
}
void tk_parser_clear(struct tk_parser *res){
    if(res->data != NULL) skr_free(res->data);
    res->data = NULL;
    res->position = res->str_size = res->line_pos = 0;

    res->current_line = 0;
    res->scope_pos = 0;
    list_clear(res->list);
    string_clear(res->error_msg);
}
void tk_parser_free(struct tk_parser *res){
    if(res->data != NULL) skr_free(res->data);
    list_free(res->list);
    string_free(res->error_msg);
    skr_free(res);
}

void tk_parser_set_str(struct tk_parser *res, char *data, size_t size){
    tk_parser_clear(res);
    res->str_size = size;
    res->data = skr_malloc(res->str_size);
    memcpy(res->data, data, size);
}
void tk_parser_set_file(struct tk_parser *res, char *file_path){
    tk_parser_clear(res);

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) return;
    fseek(fp, 0, SEEK_END);
    res->str_size = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    res->data = skr_malloc(res->str_size);
#ifdef WIN32
    char c;
    size_t i;
    for (i = 0; i < res->str_size && (c = (char)getc(fp)) != EOF; i++) {
        res->data[i] = c;
    }
    res->str_size = i;
#else
    for (size_t i = 0; i < res->str_size; i++) {
        res->data[i] = (char) getc(fp);
    }
#endif
    fclose(fp);
}