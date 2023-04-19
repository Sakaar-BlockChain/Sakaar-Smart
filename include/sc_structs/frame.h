#ifndef FRAME_H
#define FRAME_H

#include "struct.h"
#include "attrib_list.h"

struct frame_st{
    struct attrib_list_st attrib;
    struct list_st data;

    size_t counter;
};

struct frame_st *frame_new();
void frame_set(struct frame_st *, const struct frame_st *);
void frame_clear(struct frame_st *);
void frame_free(struct frame_st *);

struct frame_st *frame_copy(struct frame_st *);

void frame_load_data(struct frame_st *);
void frame_save_data(struct frame_st *);

void print_frame(const struct frame_st *, int);

#endif //FRAME_H
