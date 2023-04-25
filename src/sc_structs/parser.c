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
    res->scope_type = ScopeType_None;

    string_clear(&res->error_msg);
    res->error_pos = 0;

    closure_list_clear(&res->closures);
    node_list_clear(&res->nodes);
    token_list_clear(&res->tokens);
    variable_list_list_clear(&res->variables);

    closure_list_clear(&res->closures_stack);
    variable_list_list_clear(&res->variables_stack);
}

void parser_data_inti(struct parser_st *res) {
    res->data_str = NULL;
    res->data_size = 0;
    res->data_pos = 0;

    res->line_num = 0;
    res->line_pos = 0;

    res->scope_pos = 0;
    res->scope_type = ScopeType_None;

    string_data_init(&res->error_msg);
    res->error_pos = 0;

    closure_list_data_init(&res->closures);
    node_list_data_init(&res->nodes);
    token_list_data_init(&res->tokens);
    variable_list_list_data_init(&res->variables);
    res->closures.type = 1;
    res->nodes.type = 1;
    res->tokens.type = 1;
    res->variables.type = 1;

    closure_list_data_init(&res->closures_stack);
    variable_list_list_data_init(&res->variables_stack);
}
void parser_data_free(struct parser_st *res) {
    if(res->data_str != NULL) skr_free(res->data_str);

    string_data_free(&res->error_msg);
    res->error_pos = 0;

    closure_list_data_free(&res->closures);
    node_list_data_free(&res->nodes);
    token_list_data_free(&res->tokens);
    variable_list_list_data_free(&res->variables);

    closure_list_data_free(&res->closures_stack);
    variable_list_list_data_free(&res->variables_stack);
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


size_t parser_new_ident(struct parser_st *res, struct string_st *name) {
    struct variable_list_st *list = variable_list_list_last(&res->variables_stack);
    for (size_t i = 0; i < list->size; i++) {
        if (string_cmp(&list->variables[i]->name, name) == 0) {
            return i + 1;
        }
    }
    variable_list_add_new(list);
    string_set(&variable_list_last(list)->name, name);
    return list->size;
}
size_t parser_get_ident(struct parser_st *res, struct string_st *name) {
    struct variable_list_st *list;
    struct closure_st *closure;

    size_t i = res->variables_stack.size;
    struct variable_st *ptr = NULL;
    size_t pos = -1;

    for (; i > 0; i--) {
        list = res->variables_stack.variable_lists[i - 1];
        for (size_t j = 0; j < list->size; j++) {
            if (string_cmp(&list->variables[j]->name, name) == 0) {
                ptr = list->variables[j];
                pos = j + 1;
                break;
            }
        }
        if (ptr != NULL) break;
    }

    if (ptr == NULL) return 0;

    for (; i < res->variables_stack.size; i++) {
        if (res->closures_stack.closures[i] == NULL) continue;
        list = res->variables_stack.variable_lists[i];
        closure = res->closures_stack.closures[i];

        variable_list_add_new(list);
        string_set(&variable_list_last(list)->name, name);

        closure_append(closure, variable_list_last(list), ptr);

        ptr = variable_list_last(list);
        pos = list->size;
    }

    return pos;
}