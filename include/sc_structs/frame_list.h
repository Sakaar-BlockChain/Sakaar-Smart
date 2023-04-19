#ifndef FRAME_LIST_H
#define FRAME_LIST_H

struct frame_list_st{
    struct frame_st **frames;
    size_t size;
    size_t max_size;
};

void frame_list_set(struct frame_list_st *, const struct frame_list_st *);
void frame_list_clear(struct frame_list_st *);

void frame_list_data_init(struct frame_list_st *);
void frame_list_data_free(struct frame_list_st *);

void frame_list_resize(struct frame_list_st *, size_t);
void frame_list_append(struct frame_list_st *, struct frame_st *);
void frame_list_add_new(struct frame_list_st *);
struct frame_st *frame_list_last(struct frame_list_st *);


void print_frame_list(const struct frame_list_st *, int);

#endif //FRAME_LIST_H
