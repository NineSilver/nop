#ifndef __NOP_PAGE_H__
#define __NOP_PAGE_H__

#include <arch/values.h>
#include <stdint.h>

#define PAGE_BITMAP_SIZE (PAGE_COUNT >> 3)

#define PAGE_FREE 0
#define PAGE_USED 1

extern uint8_t *page_bitmap;

void page_init(void);
void page_mark(size_t start, size_t end, int value);

#endif
