#ifndef SN_IMPORT_H
#define SN_IMPORT_H

#include "smart.h"

#define ImprType_Name           0x01
#define ImprType_Module_As      0x02
#define ImprType_Module_All     0x03
#define ImprType_From           0x04
#define ImprType_Import         0x05

int8_t name_impr(struct parser_st *, struct node_st *);

int8_t module_impr(struct parser_st *, struct node_st *);

int8_t module_as_impr(struct parser_st *, struct node_st *);

int8_t from_impr(struct parser_st *, struct node_st *);

int8_t import_impr(struct parser_st *, struct node_st *);

#endif //SN_IMPORT_H
