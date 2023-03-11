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

char annotated_stmt(struct sc_parser *, struct ast_node *);
char assignment_stmt(struct sc_parser *, struct ast_node *);
char return_stmt(struct sc_parser *, struct ast_node *);
char break_stmt(struct sc_parser *, struct ast_node *);
char continue_stmt(struct sc_parser *, struct ast_node *);
char simple_stmt(struct sc_parser *, struct ast_node *);
char stmt_list(struct sc_parser *, struct ast_node *);

char parameter_list(struct sc_parser *, struct ast_node *);
char extends_list(struct sc_parser *, struct ast_node *);

char function_stmt(struct sc_parser *, struct ast_node *);
char public_function_stmt(struct sc_parser *, struct ast_node *);
char static_function_stmt(struct sc_parser *, struct ast_node *);
char private_function_stmt(struct sc_parser *, struct ast_node *);

char if_stmt(struct sc_parser *, struct ast_node *);
char while_stmt(struct sc_parser *, struct ast_node *);
char do_while_stmt(struct sc_parser *, struct ast_node *);
char class_stmt(struct sc_parser *, struct ast_node *);
char compound_stmt(struct sc_parser *, struct ast_node *);

char statement(struct sc_parser *, struct ast_node *);
char suite(struct sc_parser *, struct ast_node *);

#endif //AN_STMT_H
