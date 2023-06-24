#include <arch/values.h>
#include <nop/start.h>
#include <nop/page.h>
#include <alloc.h>

/* This function is not the first thing that gets called upon
   boot, instead, it's what's called right after all the
   architecture-specific blobs. */

void start(const start_block_t *blocks, int block_count, const start_device_t *devices, int device_count) {
  /* 1. Setup page bitmap allocator. */
  
  for (int i = 0; i < block_count; i++) {
    if (blocks[i].size >= PAGE_BITMAP_SIZE) {
      blocks[i].size -= PAGE_BITMAP_SIZE;
      
      page_init(blocks[i].start + blocks[i].size);
      break;
    }
  }
  
  for (int i = 0; i < block_count; i++) {
    size_t start = ((size_t)(blocks[i].start) + (PAGE_SIZE - 1)) / PAGE_SIZE;
    size_t end = ((size_t)(blocks[i].start) + blocks[i].size) / PAGE_SIZE;
    
    page_mark(start, end, PAGE_FREE);
  }
  
  /* 2. Setup kernel heap allocator. */
  
  alloc_init(...); /* TODO */
  
  /* 3. Initialize passed in devices. */
  
  for (int i = 0; i < device_count; i++) {
    /* TODO */
  }
  
  /* 4. Try to establish a console to log information to. */
  
  /* 5. Boot? */
  
  /* It's not like we can do much more here either... */
  for (;;);
}
