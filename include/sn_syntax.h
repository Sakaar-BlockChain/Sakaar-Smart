#ifndef AN_ANALIZE_H
#define AN_ANALIZE_H

#include "sn_syntax/sn_oper.h"
#include "sn_syntax/sn_stmt.h"
#include "sn_syntax/sn_expr.h"
#include "tk_tokenize.h"

#define SN_Status_Success   0
#define SN_Status_Nothing   1
#define SN_Status_Error     2

#define parser_end if (parser->list->size <= parser->pos)
#define parser_end_break    parser_end break;
#define parser_end_goto     parser_end {result = SN_Status_Error; goto end;}
#define parser_end_return   parser_end {return SN_Status_Error;}

char token_analyzer(struct sc_parser *, struct ast_node *);

#endif //AN_ANALIZE_H
