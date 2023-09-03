#ifndef AN_OPER_H
#define AN_OPER_H

#include "smart.h"

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

int8_t u_oper(struct parser_st *, struct node_st *);
int8_t m_oper(struct parser_st *, struct node_st *);
int8_t a_oper(struct parser_st *, struct node_st *);
int8_t shift_oper(struct parser_st *, struct node_st *);
int8_t and_oper(struct parser_st *, struct node_st *);
int8_t xor_oper(struct parser_st *, struct node_st *);
int8_t or_oper(struct parser_st *, struct node_st *);

int8_t comp_oper(struct parser_st *, struct node_st *);
int8_t not_test_oper(struct parser_st *, struct node_st *);
int8_t and_test_oper(struct parser_st *, struct node_st *);
int8_t or_test_oper(struct parser_st *, struct node_st *);
int8_t list_oper(struct parser_st *, struct node_st *, short, short);

int8_t list_ident(struct parser_st *, struct node_st *);

#endif //AN_OPER_H
