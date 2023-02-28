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

void u_oper(struct an_parser *, struct an_node *);
void m_oper(struct an_parser *, struct an_node *);
void a_oper(struct an_parser *, struct an_node *);
void shift_oper(struct an_parser *, struct an_node *);
void and_oper(struct an_parser *, struct an_node *);
void xor_oper(struct an_parser *, struct an_node *);
void or_oper(struct an_parser *, struct an_node *);

void comp_oper(struct an_parser *, struct an_node *);
void not_test_oper(struct an_parser *, struct an_node *);
void and_test_oper(struct an_parser *, struct an_node *);
void or_test_oper(struct an_parser *, struct an_node *);
void list_oper(struct an_parser *, struct an_node *, short, short);

void list_ident(struct an_parser *, struct an_node *, short, short);

#endif //AN_OPER_H
