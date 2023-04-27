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

int u_oper(struct parser_st *, struct node_st *);
int m_oper(struct parser_st *, struct node_st *);
int a_oper(struct parser_st *, struct node_st *);
int shift_oper(struct parser_st *, struct node_st *);
int and_oper(struct parser_st *, struct node_st *);
int xor_oper(struct parser_st *, struct node_st *);
int or_oper(struct parser_st *, struct node_st *);

int comp_oper(struct parser_st *, struct node_st *);
int not_test_oper(struct parser_st *, struct node_st *);
int and_test_oper(struct parser_st *, struct node_st *);
int or_test_oper(struct parser_st *, struct node_st *);
int list_oper(struct parser_st *, struct node_st *, short, short);

int list_ident(struct parser_st *, struct node_st *);

#endif //AN_OPER_H
