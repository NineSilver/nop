#include <nop/arch/values.h>
#include <nop/page.h>
#include <nop/log.h>
#include <stdint.h>

uint8_t *page_bitmap;

void page_init(void *bitmap) {
  log("[page] Page bitmap at 0x%08X.\n", bitmap);
  
  page_bitmap = bitmap;
  page_mark(0, PAGE_COUNT, PAGE_USED);
}

void page_mark(size_t start, size_t end, int value) {
  while ((start & 7) && start < end) {
    if (value) {
      page_bitmap[start >> 3] |= (1 << (start & 7));
    } else {
      page_bitmap[start >> 3] &= ~(1 << (start & 7));
    }
    
    start++;
  }
  
  while ((end & 7) && start < end) {
    end--;
    
    if (value) {
      page_bitmap[end >> 3] |= (1 << (end & 7));
    } else {
      page_bitmap[end >> 3] &= ~(1 << (end & 7));
    }
  }
  
  while (start < end) {
    page_bitmap[start >> 3] = (value ? 0xFF : 0x00);
    start += 8;
  }
}

void *page_alloc(size_t n, size_t step) {
  /* TODO: Speed up allocation by a free chunk table or
     something? */
  
  step = (step + (PAGE_SIZE - 1)) / PAGE_SIZE;
  
  size_t i;
  
  for (i = 0; i <= PAGE_COUNT - n; i += step) {
    size_t free_pages = 0, free_start = i;
    
    while (free_pages < n && !(page_bitmap[i >> 3] & (1 << (i & 7)))) {
      free_pages++, i++;
    }
    
    if (free_pages >= n) {
      page_mark(free_start, free_start + n, PAGE_USED);
      return (void *)(free_start * PAGE_SIZE);
    }
  }
  
  log("[page] Could not allocate %u pages: Not enough contiguous free space.\n", n);
  return NULL;
}

void page_free(void *ptr, size_t n) {
  size_t start = (size_t)(ptr) / PAGE_SIZE;
  page_mark(start, start + n, PAGE_FREE);
}
