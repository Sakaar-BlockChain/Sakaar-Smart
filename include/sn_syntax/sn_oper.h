#ifndef AN_OPER_H
#define AN_OPER_H

#include "sc_structs.h"

#define ExprType_U              0x01
#define ExprType_M              0x02
#define ExprType_A              0x03
#define ExprType_Shift          0x04
#define ExprType_And            0x05
#define ExprType_Xor            0x06
#define ExprType_Or             0x07
#define ExprType_Comp           0x08
#define ExprType_NotTest        0x09
#define ExprType_AndTest        0x0a
#define ExprType_OrTest         0x0b

char u_oper(struct sc_parser *, struct ast_node *);
char m_oper(struct sc_parser *, struct ast_node *);
char a_oper(struct sc_parser *, struct ast_node *);
char shift_oper(struct sc_parser *, struct ast_node *);
char and_oper(struct sc_parser *, struct ast_node *);
char xor_oper(struct sc_parser *, struct ast_node *);
char or_oper(struct sc_parser *, struct ast_node *);

char comp_oper(struct sc_parser *, struct ast_node *);
char not_test_oper(struct sc_parser *, struct ast_node *);
char and_test_oper(struct sc_parser *, struct ast_node *);
char or_test_oper(struct sc_parser *, struct ast_node *);
char list_oper(struct sc_parser *, struct ast_node *, short, short);

char list_ident(struct sc_parser *, struct ast_node *);

#endif //AN_OPER_H
