#ifndef AN_ANALIZE_H
#define AN_ANALIZE_H

#include "sn_syntax/sn_oper.h"
#include "sn_syntax/sn_stmt.h"
#include "sn_syntax/sn_expr.h"
#include "tk_tokenize.h"

#define SN_Status_Success       0
#define SN_Status_Nothing       1
#define SN_Status_EOF           2
#define SN_Status_Error         3
#define SN_Status_Error_Class   4
#define SN_Status_Error_Func    5
#define SN_Status_Error_Loop    6
#define SN_Status_Error_Indent  7

int token_analyzer(struct parser_st *, struct node_st *);

#endif //AN_ANALIZE_H
