#ifndef CG_GENERATION_H
#define CG_GENERATION_H

void cg_generate_code(struct parser_st *parser, struct node_st *node, struct bytecode_st *code);
void cg_generate(struct parser_st *parser);

#endif //CG_GENERATION_H
