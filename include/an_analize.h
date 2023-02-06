#ifndef AN_ANALIZE_H
#define AN_ANALIZE_H

#include "an_analize/an_oper.h"
#include "an_analize/an_stmt.h"
#include "an_analize/an_expr.h"

void token_analyzer(struct an_parser *, struct an_node *);

#endif //AN_ANALIZE_H
