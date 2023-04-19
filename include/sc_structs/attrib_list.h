#ifndef ATTRIB_LIST_H
#define ATTRIB_LIST_H

#include "struct.h"

struct attrib_list_st{
    struct attrib_st **attribs;
    size_t size;
    size_t max_size;
};

void attrib_list_set(struct attrib_list_st *, const struct attrib_list_st *);
void attrib_list_clear(struct attrib_list_st *);

void attrib_list_data_init(struct attrib_list_st *);
void attrib_list_data_free(struct attrib_list_st *);

void attrib_list_resize(struct attrib_list_st *, size_t);
void attrib_list_append(struct attrib_list_st *, struct attrib_st *);
void attrib_list_add_new(struct attrib_list_st *);
struct attrib_st *attrib_list_last(struct attrib_list_st *);


void print_attrib_list(const struct attrib_list_st *, int);

#endif //ATTRIB_LIST_H
