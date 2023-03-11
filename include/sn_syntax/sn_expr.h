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

char scopes_expr(struct sc_parser *, struct ast_node *);
char list_expr(struct sc_parser *, struct ast_node *);
char ident_get_expr(struct sc_parser *, struct ast_node *);
char ident_new_expr(struct sc_parser *, struct ast_node *);
char bool_expr(struct sc_parser *, struct ast_node *);
char number_expr(struct sc_parser *, struct ast_node *);
char string_expr(struct sc_parser *, struct ast_node *);
char null_expr(struct sc_parser *, struct ast_node *);
char literal_expr(struct sc_parser *, struct ast_node *);
char atom_expr(struct sc_parser *, struct ast_node *);
char primary_expr(struct sc_parser *, struct ast_node *);

#endif //AN_EXPR_H
