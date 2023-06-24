#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <stdint.h>

void alloc_init(void *(*block_alloc)(size_t *), void (*block_free)(void *, size_t));
void alloc_tidy(void);
void alloc_block(size_t n);

void *malloc_once(size_t n);

void *malloc(size_t n);
void *realloc(void *ptr, size_t n);
void  free(void *ptr);
