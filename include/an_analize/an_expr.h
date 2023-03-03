#ifndef AN_EXPR_H
#define AN_EXPR_H

#include "sc_structs.h"

#define Literal_None            0x00
#define Literal_String          0x01
#define Literal_Number          0x02

#define PrimType_List           0x01
#define PrimType_Ident_get      0x02
#define PrimType_Ident_new      0x03
#define PrimType_Literal        0x04
#define PrimType_Attrib         0x05
#define PrimType_Subscript      0x06
#define PrimType_Call           0x07

void scopes_expr(struct sc_parser *, struct an_node *);
void list_expr(struct sc_parser *, struct an_node *);
void ident_get_expr(struct sc_parser *, struct an_node *);
void ident_new_expr(struct sc_parser *, struct an_node *);
void bool_expr(struct sc_parser *, struct an_node *);
void number_expr(struct sc_parser *, struct an_node *);
void string_expr(struct sc_parser *, struct an_node *);
void null_expr(struct sc_parser *, struct an_node *);
void literal_expr(struct sc_parser *, struct an_node *);
void atom_expr(struct sc_parser *, struct an_node *);
void primary_expr(struct sc_parser *, struct an_node *);

#endif //AN_EXPR_H
