#ifndef TK_IDENTIFIER_H
#define TK_IDENTIFIER_H

#include "smart.h"


#define SpaceChar(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IdentifierStart(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || (c) == '_')
#define IdentifierChar(c) (IdentifierStart(c) || ((c) >= '0' && (c) <= '9'))


void tokenize_identifier(struct token_st *, struct parser_st *);

#endif //TK_IDENTIFIER_H
