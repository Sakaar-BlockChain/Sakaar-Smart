#ifndef AN_STMT_H
#define AN_STMT_H

#include "smart.h"


#define StmtType_Annot          0x01
#define StmtType_Assign         0x02
#define StmtType_Return         0x03
#define StmtType_Break          0x04
#define StmtType_Continue       0x05
#define StmtType_Oper           0x06

#define StmtType_Params         0x11
#define StmtType_Extends        0x12

#define StmtType_Func           0x20
#define StmtType_Func_Body      0x21

#define StmtType_Class          0x30
#define StmtType_Class_Body     0x31

#define StmtType_If             0x41
#define StmtType_While          0x42
#define StmtType_DoWhile        0x43
#define StmtType_TryWith        0x44

#define StmtType_List           0x51

int8_t annotated_stmt(struct parser_st *, struct node_st *);
int8_t assignment_stmt(struct parser_st *, struct node_st *);
int8_t return_stmt(struct parser_st *, struct node_st *);
int8_t break_stmt(struct parser_st *, struct node_st *);
int8_t continue_stmt(struct parser_st *, struct node_st *);
int8_t simple_stmt(struct parser_st *, struct node_st *);
int8_t stmt_list(struct parser_st *, struct node_st *);

int8_t parameter_list(struct parser_st *, struct node_st *);
int8_t extends_list(struct parser_st *, struct node_st *);

int8_t function_stmt(struct parser_st *, struct node_st *);
int8_t function_body_stmt(struct parser_st *, struct node_st *);

int8_t class_stmt(struct parser_st *, struct node_st *);
int8_t class_body_stmt(struct parser_st *, struct node_st *);

int8_t if_stmt(struct parser_st *, struct node_st *);
int8_t while_stmt(struct parser_st *, struct node_st *);
int8_t do_while_stmt(struct parser_st *, struct node_st *);
int8_t try_with_stmt(struct parser_st *, struct node_st *);
int8_t compound_stmt(struct parser_st *, struct node_st *);

int8_t statement(struct parser_st *, struct node_st *);
int8_t suite(struct parser_st *parser, struct node_st *expr);

#endif //AN_STMT_H
