#ifndef AN_EXPR_H
#define AN_EXPR_H

#include "smart.h"

#define PrimType_List           0x01
#define PrimType_Ident_get      0x02
#define PrimType_Ident_new      0x03
#define PrimType_Literal        0x04
#define PrimType_Attrib         0x05
#define PrimType_Subscript      0x06
#define PrimType_Call           0x07

int8_t scopes_expr(struct parser_st *, struct node_st *);

int8_t list_expr(struct parser_st *, struct node_st *);

int8_t ident_get_expr(struct parser_st *, struct node_st *);

int8_t ident_new_expr(struct parser_st *, struct node_st *);

int8_t bool_expr(struct parser_st *, struct node_st *);

int8_t number_expr(struct parser_st *, struct node_st *);

int8_t string_expr(struct parser_st *, struct node_st *);

int8_t null_expr(struct parser_st *, struct node_st *);

int8_t literal_expr(struct parser_st *, struct node_st *);

int8_t atom_expr(struct parser_st *, struct node_st *);

int8_t primary_expr(struct parser_st *, struct node_st *);

int8_t set_primary_expr(struct parser_st *, struct node_st *);

#endif //AN_EXPR_H
