#include "sc_structs.h"

struct sc_error *sc_error_new() {
    struct sc_error *res = skr_malloc(sizeof(struct sc_error));
    res->present = 0;
    res->line_num = 0;
    res->line_pos = 0;
    res->pos = 0;

    res->type = (struct string_st) {NULL, 0, 0};
    res->msg = (struct string_st) {NULL, 0, 0};
    return res;
}
void sc_error_set(struct sc_error *res, const struct sc_error *a) {
    res->present = a->present;
    res->line_num = a->line_num;
    res->line_pos = a->line_pos;
    res->pos = a->pos;

    string_set(&res->type, &a->type);
    string_set(&res->msg, &a->msg);
}
void sc_error_clear(struct sc_error *res) {
    res->present = 0;
    res->line_num = 0;
    res->line_pos = 0;
    res->pos = 0;

    string_clear(&res->type);
    string_clear(&res->msg);
}
void sc_error_free(struct sc_error *res) {
    string_data_free(&res->type);
    string_data_free(&res->msg);
    skr_free(res);
}

void sc_error_set_pos(struct sc_error *res, size_t line_num, size_t line_pos, size_t pos) {
    res->line_num = line_num;
    res->line_pos = line_pos;
    res->pos = pos;
}
void sc_error_set_msg(struct sc_error *res, char *type, char *msg) {
    res->present = 1;
    string_set_str(&res->type, type, strlen(type));
    string_set_str(&res->msg, msg, strlen(msg));
}