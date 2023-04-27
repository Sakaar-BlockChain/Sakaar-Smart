#include "sc_structs.h"
#ifdef WIN32
#include <stdio.h>
#endif


void parser_clear(struct parser_st *res) {
    string_clear(&res->file_name);
    string_clear(&res->file_path);
    if(res->data_str != NULL) skr_free(res->data_str);
    res->data_str = NULL;
    res->data_size = 0;
    res->data_pos = 0;

    res->line_num = 0;
    res->line_pos = 0;

    res->scope_pos = 0;
    res->var_start_pos = 0;
    res->scope_type = ScopeType_None;

    string_clear(&res->error_msg);
    res->error_pos = 0;

    bytecode_list_clear(&res->codes);
    closure_list_clear(&res->closures);
    node_list_clear(&res->nodes);
    token_list_clear(&res->tokens);
    variable_list_list_clear(&res->variables);

    bytecode_list_clear(&res->codes_stack);
    closure_list_clear(&res->closures_stack);
    variable_list_list_clear(&res->variables_stack);

    list_clear(res->const_objects);
    list_add_new(res->const_objects, NONE_TYPE);
    list_clear(res->temp_stack);
    list_clear(res->var_stack);
}

void parser_data_inti(struct parser_st *res) {
    string_data_init(&res->file_name);
    string_data_init(&res->file_path);
    res->data_str = NULL;
    res->data_size = 0;
    res->data_pos = 0;

    res->line_num = 0;
    res->line_pos = 0;

    res->scope_pos = 0;
    res->var_start_pos = 0;
    res->scope_type = ScopeType_None;

    string_data_init(&res->error_msg);
    res->error_pos = 0;

    bytecode_list_data_init(&res->codes);
    closure_list_data_init(&res->closures);
    node_list_data_init(&res->nodes);
    token_list_data_init(&res->tokens);
    variable_list_list_data_init(&res->variables);
    res->codes.type = 1;
    res->closures.type = 1;
    res->nodes.type = 1;
    res->tokens.type = 1;
    res->variables.type = 1;

    bytecode_list_data_init(&res->codes_stack);
    closure_list_data_init(&res->closures_stack);
    variable_list_list_data_init(&res->variables_stack);

    res->const_objects = list_new();
    list_add_new(res->const_objects, NONE_TYPE);
    res->temp_stack = list_new();
    res->var_stack = list_new();
}
void parser_data_free(struct parser_st *res) {
    string_data_free(&res->file_name);
    string_data_free(&res->file_path);
    if(res->data_str != NULL) skr_free(res->data_str);

    string_data_free(&res->error_msg);
    res->error_pos = 0;

    bytecode_list_data_free(&res->codes);
    closure_list_data_free(&res->closures);
    node_list_data_free(&res->nodes);
    token_list_data_free(&res->tokens);
    variable_list_list_data_free(&res->variables);

    bytecode_list_data_free(&res->codes_stack);
    closure_list_data_free(&res->closures_stack);
    variable_list_list_data_free(&res->variables_stack);

    list_free(res->const_objects);
    list_free(res->temp_stack);
    list_free(res->var_stack);
}

void parser_set_file(struct parser_st *res, char *file_path){
    parser_clear(res);

    if (memcmp(file_path + strlen(file_path) - 3, ".sc", 3) != 0) return string_set_str(&res->error_msg, "File Not Exist", 14);
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) return string_set_str(&res->error_msg, "File Not Exist", 14);
    fseek(fp, 0, SEEK_END);
    res->data_size = ftell(fp);


    char *name = NULL;
#ifdef WIN32
    (name = strrchr(file_path, '\\')) ? ++name : ((name = strrchr(file_path, '/')) ? ++name : (name = file_path));
#else
    (name = strrchr(file_path, '/')) ? ++name : (name = file_path);
#endif
    printf("name : %s\n",name);
    string_set_str(&res->file_name, name, strlen(name));
    string_set_str(&res->file_path, file_path, strlen(file_path) - strlen(name));




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
size_t parser_const_obj(struct parser_st *res, struct object_st *obj) {
    for(size_t i=0; i<res->const_objects->size;i++){
        if (object_cmp(res->const_objects->data[i], obj) == 0) {
            object_free(obj);
            return i;
        }
    }
    list_append(res->const_objects, obj);
    object_free(obj);
    return res->const_objects->size - 1;
}
size_t parser_codespace(struct parser_st *res) {
    bytecode_list_add_new(&res->codes);
    return res->codes.size - 1;
}

void parser_store_vars(struct parser_st *res, size_t size, size_t position) {
    list_add_new(res->var_stack, INTEGER_TYPE);
    integer_set_ui(res->var_stack->data[res->var_stack->size-1]->data, res->var_start_pos);
    list_add_new(res->var_stack, INTEGER_TYPE);
    integer_set_ui(res->var_stack->data[res->var_stack->size-1]->data, position);
    res->var_start_pos = res->var_stack->size - 1;
    list_resize(res->var_stack, res->var_start_pos + 1 + size);
}
size_t parser_restore_vars(struct parser_st *res) {
    size_t size = res->var_start_pos;
    size_t position = integer_get_ui(res->var_stack->data[res->var_start_pos]->data);
    res->var_start_pos = integer_get_ui(res->var_stack->data[res->var_start_pos - 1]->data);
    list_resize(res->var_stack, size - 1);
    return position;
}