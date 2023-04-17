#include "sc_structs.h"
#ifdef WIN32
#include <stdio.h>
#endif


void parser_clear(struct parser_st *res) {
    if(res->data_str != NULL) skr_free(res->data_str);
    res->data_str = NULL;
    res->data_size = 0;
    res->data_pos = 0;

    res->line_num = 0;
    res->line_pos = 0;

    res->scope_pos = 0;

    string_clear(&res->error_msg);
    res->error_pos = 0;

    token_list_clear(&res->tokens);
    node_list_clear(&res->nodes);
    closure_list_clear(&res->closures);
}

void parser_data_inti(struct parser_st *res) {
    res->data_str = NULL;
    res->data_size = 0;
    res->data_pos = 0;

    res->line_num = 0;
    res->line_pos = 0;

    res->scope_pos = 0;

    string_data_init(&res->error_msg);
    res->error_pos = 0;

    token_list_data_init(&res->tokens);
    res->tokens.type = 1;
    node_list_data_init(&res->nodes);
    res->nodes.type = 1;
    closure_list_data_init(&res->closures);
    res->closures.type = 1;
}
void parser_data_free(struct parser_st *res) {
    if(res->data_str != NULL) skr_free(res->data_str);

    string_data_free(&res->error_msg);
    res->error_pos = 0;

    token_list_data_free(&res->tokens);
    node_list_data_free(&res->nodes);
    closure_list_data_free(&res->closures);
}

void parser_set_file(struct parser_st *res, char *file_path){
    parser_clear(res);

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) return;
    fseek(fp, 0, SEEK_END);
    res->data_size = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    res->data_str = skr_malloc(res->data_size);
#ifdef WIN32
    char c;
    size_t i;
    for (i = 0; i < res->data_size && (c = (char)getc(fp)) != EOF; i++) {
        res->data_str[i] = c;
    }
    res->data_size = i;
#else
    for (size_t i = 0; i < res->data_size; i++) {
        res->data_str[i] = (char) getc(fp);
    }
#endif
    fclose(fp);
}
void parser_set_str(struct parser_st *res, char *data, size_t size) {
    parser_clear(res);

    res->data_size = size;
    res->data_str = skr_malloc(res->data_size);
    memcpy(res->data_str, data, size);
}