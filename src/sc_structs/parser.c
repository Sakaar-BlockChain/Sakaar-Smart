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

    if(res->return_obj != NULL) object_free(res->return_obj);
    if(res->error_obj != NULL) object_free(res->error_obj);
    res->return_obj = NULL;
    res->error_obj = NULL;

    res->interrupt_type = Interrupt_None;

    block_list_clear(&res->blocks);
    closure_list_clear(&res->closures);
    node_list_clear(&res->nodes);
    token_list_clear(&res->tokens);
    variable_list_clear(&res->variables);

    block_list_clear(&res->blocks_stack);
    closure_list_clear(&res->closures_stack);
    frame_list_clear(&res->frame_stack);
    variable_list_clear(&res->variables_stack);

    list_clear(res->temp_memory);
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

    res->return_obj = NULL;
    res->error_obj = NULL;

    block_list_data_init(&res->blocks);
    closure_list_data_init(&res->closures);
    node_list_data_init(&res->nodes);
    token_list_data_init(&res->tokens);
    variable_list_data_init(&res->variables);
    res->blocks.type = 1;
    res->closures.type = 1;
    res->nodes.type = 1;
    res->tokens.type = 1;
    res->variables.type = 1;

    res->interrupt_type = Interrupt_None;

    block_list_data_init(&res->blocks_stack);
    closure_list_data_init(&res->closures_stack);
    frame_list_data_init(&res->frame_stack);
    variable_list_data_init(&res->variables_stack);

    res->temp_memory = list_new();
}
void parser_data_free(struct parser_st *res) {
    if(res->data_str != NULL) skr_free(res->data_str);

    string_data_free(&res->error_msg);
    res->error_pos = 0;

    if(res->return_obj != NULL) object_free(res->return_obj);
    if(res->error_obj != NULL) object_free(res->error_obj);

    block_list_data_free(&res->blocks);
    closure_list_data_free(&res->closures);
    node_list_data_free(&res->nodes);
    token_list_data_free(&res->tokens);
    variable_list_data_free(&res->variables);

    block_list_data_free(&res->blocks_stack);
    closure_list_data_free(&res->closures_stack);
    frame_list_data_free(&res->frame_stack);
    variable_list_data_free(&res->variables_stack);

    list_free(res->temp_memory);
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


struct attrib_st *parser_new_ident(struct parser_st *res, struct string_st *name) {
    struct attrib_list_st *list = &variable_list_last(&res->variables_stack)->attrib;
    for (size_t i = 0; i < list->size; i++) {
        if (string_cmp(&list->attribs[i]->name, name) == 0) {
            return attrib_copy(list->attribs[i]);
        }
    }
    attrib_list_add_new(list);
    string_set(&attrib_list_last(list)->name, name);
    return attrib_copy(attrib_list_last(list));
}
struct attrib_st *parser_get_ident(struct parser_st *res, struct string_st *name) {
    struct attrib_list_st *list;
    struct closure_st *closure;

    size_t i = res->variables_stack.size;
    struct attrib_st *ptr = NULL;

    for (; i > 0; i--) {
        list = &res->variables_stack.variables[i - 1]->attrib;
        for (size_t j = 0; j < list->size; j++) {
            if (string_cmp(&list->attribs[j]->name, name) == 0) {
                ptr = list->attribs[j];
                break;
            }
        }
        if (ptr != NULL) break;
    }

    if (ptr == NULL) return NULL;

    for (; i < res->variables_stack.size; i++) {
        if (res->closures_stack.closures[i] == NULL) continue;
        list = &res->variables_stack.variables[i]->attrib;
        closure = res->closures_stack.closures[i];

        attrib_list_add_new(list);
        string_set(&attrib_list_last(list)->name, name);

        closure_append(closure, attrib_list_last(list), ptr);

        ptr = attrib_list_last(list);
    }

    return attrib_copy(ptr);
}