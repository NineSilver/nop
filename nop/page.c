#include <arch/values.h>
#include <nop/page.h>
#include <stdint.h>

uint8_t *page_bitmap;

void page_init(void *bitmap) {
  page_bitmap = bitmap;
  page_mark(0, PAGE_COUNT, PAGE_USED);
}

void page_mark(size_t start, size_t end, int value) {
  /* TODO */
}
