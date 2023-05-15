#ifndef SAKAAR_SC_ERROR_H
#define SAKAAR_SC_ERROR_H

#include "struct.h"

#define ErrorType_Import    "Import Error\0"
#define ErrorType_Tokenizer "Tokenizer Error\0"
#define ErrorType_Syntax    "Syntax Error\0"
#define ErrorType_Semantic  "Semantic Error\0"

struct sc_error{
    int present;
    size_t line_num, line_pos, pos;

    struct string_st msg;
    struct string_st type;
};

struct sc_error *sc_error_new();
void sc_error_set(struct sc_error *, const struct sc_error *);
void sc_error_clear(struct sc_error *);
void sc_error_free(struct sc_error *);

void sc_error_set_pos(struct sc_error *, size_t, size_t, size_t);
void sc_error_set_msg(struct sc_error *, char *, char *);

#endif //SAKAAR_SC_ERROR_H
