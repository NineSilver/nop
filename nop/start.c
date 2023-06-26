#include <nop/arch/values.h>
#include <nop/device.h>
#include <nop/start.h>
#include <nop/page.h>
#include <nop/fs.h>
#include <string.h>
#include <alloc.h>

static void *alloc_page_alloc(size_t *n) {
  size_t count = (*n + (PAGE_SIZE - 1)) / PAGE_SIZE;
  *n = count * PAGE_SIZE;
  
  return page_alloc(count);
}

static void alloc_page_free(void *ptr, size_t n) {
  page_free(ptr, (n + (PAGE_SIZE - 1)) / PAGE_SIZE);
}

/* This function is not the first thing that gets called upon
   boot, instead, it's what's called right after all the
   architecture-specific blobs. */

void start(start_block_t *blocks, int block_count, start_task_t *tasks, int task_count) {
  int i;
  
  /* 1. Setup page bitmap allocator. */
  
  for (i = 0; i < block_count; i++) {
    if (blocks[i].size >= PAGE_BITMAP_SIZE) {
      blocks[i].size -= PAGE_BITMAP_SIZE;
      
      page_init(blocks[i].start + blocks[i].size);
      break;
    }
  }
  
  for (i = 0; i < block_count; i++) {
    size_t start = ((size_t)(blocks[i].start) + (PAGE_SIZE - 1)) / PAGE_SIZE;
    size_t end = ((size_t)(blocks[i].start) + blocks[i].size) / PAGE_SIZE;
    
    page_mark(start, end, PAGE_FREE);
  }
  
  /* 2. Setup kernel heap allocator. */
  
  alloc_init(alloc_page_alloc, alloc_page_free, 262144);
  
  /* 3. Run all initialization tasks. */
  
  for (i = 0; i < task_count; i++) {
    tasks[i].func();
  }
  
  /* 4. Setup filesystem handling (VFS). */
  
  fs_init();
  
  /*
  "$hdd0/0/boot/grub.cfg"
  "$eth0/"
  "$cdrom0/autorun.inf"
  "$mouse0"
  "$logs"
  */
  
  /* 5. Try to establish a console to log information to. */
  
  /*
  log_init();
  log(LOG_INFO, "Hello, world!\n");
  */
  
  const char *text = "Hello, world!\n";
  
  int console_id = device_find("serial0");
  device_write(console_id, text, strlen(text));
  
  /* 6. Boot? */
  
  int file_id = fs_open("$hdd0/0/test.txt");
  
  device_write(file_id, text, strlen(text));
  device_commit(file_id);
  
  fs_close(file_id);
  
  /* It's not like we can do much more here either... */
  for (;;);
}
