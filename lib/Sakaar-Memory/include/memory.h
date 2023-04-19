#ifndef MEMORY_H
#define MEMORY_H

#include "stdlib.h"
#include <malloc.h>
#include <string.h>

struct mem_ctx {
    struct mem_arena *first_arena;
    size_t filled;
};
extern struct mem_ctx mem_ctx;

void *SKR_malloc(struct mem_ctx *ctx, size_t size);
void *SKR_realloc(struct mem_ctx *ctx, void *data, size_t size);
void SKR_free(struct mem_ctx *ctx, void *data);

//#define skr_malloc(size) SKR_malloc(&mem_ctx, size)
//#define skr_realloc(data, size) SKR_realloc(&mem_ctx, data, size)
//#define skr_free(data) SKR_free(&mem_ctx, data)

#define skr_malloc(size) malloc(size)
#define skr_realloc(data, size) realloc(data, size)
#define skr_free(data) free(data)

#endif //MEMORY_H
