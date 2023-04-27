#ifndef SN_IMPORT_H
#define SN_IMPORT_H

#include "sc_structs.h"

#define ImprType_Name           0x01
#define ImprType_Module_As      0x02
#define ImprType_Module_All     0x03
#define ImprType_From           0x04
#define ImprType_Import         0x05

int name_impr(struct parser_st *, struct node_st *);
int module_impr(struct parser_st *, struct node_st *);
int module_as_impr(struct parser_st *, struct node_st *);
int from_impr(struct parser_st *, struct node_st *);
int import_impr(struct parser_st *, struct node_st *);

#endif //SN_IMPORT_H
