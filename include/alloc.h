#ifndef __ALLOC_H__
#define __ALLOC_H__

#include <stdint.h>

void   alloc_init(void *(*block_alloc)(size_t *), void (*block_free)(void *, size_t));
size_t alloc_usage(void);

void *malloc(size_t n);
void *realloc(void *ptr, size_t n);
void  free(void *ptr);
