#ifndef AN_STMT_H
#define AN_STMT_H

#include "sc_structs.h"


#define StmtType_Annot          0x01
#define StmtType_Assign         0x02
#define StmtType_Return         0x03
#define StmtType_Break          0x04
#define StmtType_Continue       0x05

#define StmtType_Params         0x11
#define StmtType_Extends        0x12

#define StmtType_Func           0x20
#define StmtType_PUB_Func       0x21
#define StmtType_STC_Func       0x22
#define StmtType_PRI_Func       0x23

#define StmtType_If             0x31
#define StmtType_While          0x32
#define StmtType_DoWhile        0x33
#define StmtType_Class          0x34

#define StmtType_List           0x41

void annotated_stmt(struct sc_parser *, struct an_node *);
void assignment_stmt(struct sc_parser *, struct an_node *);
void return_stmt(struct sc_parser *, struct an_node *);
void break_stmt(struct sc_parser *, struct an_node *);
void continue_stmt(struct sc_parser *, struct an_node *);
void simple_stmt(struct sc_parser *, struct an_node *);
void stmt_list(struct sc_parser *, struct an_node *);

void parameter_list(struct sc_parser *, struct an_node *);
void extends_list(struct sc_parser *, struct an_node *);

void function_stmt(struct sc_parser *, struct an_node *);
void public_function_stmt(struct sc_parser *, struct an_node *);
void static_function_stmt(struct sc_parser *, struct an_node *);
void private_function_stmt(struct sc_parser *, struct an_node *);

void if_stmt(struct sc_parser *, struct an_node *);
void while_stmt(struct sc_parser *, struct an_node *);
void do_while_stmt(struct sc_parser *, struct an_node *);
void class_stmt(struct sc_parser *, struct an_node *);
void compound_stmt(struct sc_parser *, struct an_node *);

void statement(struct sc_parser *, struct an_node *);
void suite(struct sc_parser *, struct an_node *);

#endif //AN_STMT_H
