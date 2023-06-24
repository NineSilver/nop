#include <arch/values.h>
#include <nop/start.h>
#include <nop/page.h>
#include <alloc.h>

/* This function is not the first thing that gets called upon
   boot, instead, it's what's called right after all the
   architecture-specific blobs. */

void start(const start_block_t *blocks, int block_count, const start_task_t *tasks, int task_count) {
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
  
  /* 3. Setup device and filesystem handling. */
  
  /* TODO */
  
  /* 4. Run all initialization tasks. */
  
  for (int i = 0; i < task_count; i++) {
    tasks[i].func();
  }
  
  /*
  "/.hdd0/0/boot/grub.cfg"
  "/.eth0/"
  "/.cdrom0/autorun.inf"
  "/.mouse0"
  */
  
  /* 5. Try to establish a console to log information to. */
  
  /*
  log_init();
  log(LOG_INFO, "Hello, world!\n");
  */
  
  
  
  /* 6. Boot? */
  
  /* It's not like we can do much more here either... */
  for (;;);
}
