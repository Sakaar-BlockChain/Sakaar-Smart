#ifndef TK_TOKENIZE_H
#define TK_TOKENIZE_H

#include "tk_tokenize/tk_identifier.h"
#include "tk_tokenize/tk_integer.h"
#include "tk_tokenize/tk_keyword.h"
#include "tk_tokenize/tk_special.h"
#include "tk_tokenize/tk_string.h"

void tokenize_parse(struct token_st *token, struct parser_st *parser);
void tokenize(struct parser_st *parser);

#endif //TK_TOKENIZE_H
