#ifndef __NOP_PAGE_H__
#define __NOP_PAGE_H__

#include <nop/arch/values.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_BITMAP_SIZE (PAGE_COUNT >> 3)

#define PAGE_FREE 0
#define PAGE_USED 1

extern uint8_t *page_bitmap;

void  page_init(void *bitmap);
void  page_mark(size_t start, size_t end, int value);

void *page_alloc(size_t n);
void  page_free(void *ptr, size_t n);

#endif
