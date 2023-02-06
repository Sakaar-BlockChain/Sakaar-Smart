#ifndef AN_STMT_H
#define AN_STMT_H

#include "sc_structs.h"


#define StmtType_If             0x01
#define StmtType_While          0x02
#define StmtType_DoWhile        0x03
#define StmtType_For            0x04
#define StmtType_ForHeader      0x05
#define StmtType_Try            0x06
#define StmtType_Params         0x07
#define StmtType_Func           0x08
#define StmtType_Class          0x09
#define StmtType_Assign         0x0a
#define StmtType_Return         0x0b
#define StmtType_Break          0x0c
#define StmtType_Continue       0x0d
#define StmtType_Import         0x0e
#define StmtType_List           0x0f
#define StmtType_Extends        0x10

void assignment_stmt(struct an_parser *, struct an_node *);
void return_stmt(struct an_parser *, struct an_node *);
void break_stmt(struct an_parser *, struct an_node *);
void continue_stmt(struct an_parser *, struct an_node *);
void simple_stmt(struct an_parser *, struct an_node *);
void stmt_list(struct an_parser *, struct an_node *);

void for_header1_stmt(struct an_parser *, struct an_node *);
void for_header2_stmt(struct an_parser *, struct an_node *);

void for_stmt(struct an_parser *, struct an_node *);
void if_stmt(struct an_parser *, struct an_node *);
void while_stmt(struct an_parser *, struct an_node *);
void do_while_stmt(struct an_parser *, struct an_node *);
void extends_list(struct an_parser *, struct an_node *);
void parameter_list(struct an_parser *, struct an_node *);
void function_stmt(struct an_parser *, struct an_node *);
void class_stmt(struct an_parser *, struct an_node *);
void compound_stmt(struct an_parser *, struct an_node *);

void statement(struct an_parser *, struct an_node *);
void suite(struct an_parser *, struct an_node *);

#endif //AN_STMT_H
