#ifndef AN_ANALIZE_H
#define AN_ANALIZE_H

#include "sn_syntax/sn_oper.h"
#include "sn_syntax/sn_stmt.h"
#include "sn_syntax/sn_expr.h"
#include "tk_tokenize.h"

void token_analyzer(struct sc_parser *, struct ast_node *);

#endif //AN_ANALIZE_H
