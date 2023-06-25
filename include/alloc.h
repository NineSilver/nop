#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <stddef.h>

void alloc_init(void *(*page_alloc)(size_t *), void (*page_free)(void *, size_t), size_t block_min);
void alloc_tidy(void);
void alloc_block(size_t n);

void *malloc_once(size_t n);

void *malloc(size_t n);
void *realloc(void *ptr, size_t n);
void  free(void *ptr);

#endif
