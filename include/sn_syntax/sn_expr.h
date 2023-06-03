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

int scopes_expr(struct parser_st *, struct node_st *);

int list_expr(struct parser_st *, struct node_st *);

int ident_get_expr(struct parser_st *, struct node_st *);

int ident_new_expr(struct parser_st *, struct node_st *);

int bool_expr(struct parser_st *, struct node_st *);

int number_expr(struct parser_st *, struct node_st *);

int string_expr(struct parser_st *, struct node_st *);

int null_expr(struct parser_st *, struct node_st *);

int literal_expr(struct parser_st *, struct node_st *);

int atom_expr(struct parser_st *, struct node_st *);

int primary_expr(struct parser_st *, struct node_st *);

int set_primary_expr(struct parser_st *, struct node_st *);

#endif //AN_EXPR_H
