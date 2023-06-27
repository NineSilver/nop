#include <nop/arch/values.h>
#include <nop/device.h>
#include <nop/start.h>
#include <nop/page.h>
#include <nop/tree.h>
#include <nop/log.h>
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

void start(start_block_t *blocks, size_t block_count, start_task_t *tasks, size_t task_count) {
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
  
  /* 3. Create "logs" device, exiting early log mode. */
  
  log_init("logs");
  
  /* 4. Setup filesystem handling (VFS). */
  
  tree_init(&root, -1);
  
  /* 5. Run all initialization tasks. */
  
  for (i = 0; i < task_count; i++) {
    tasks[i].handle();
  }
  
  /*
  "$disk0/0/boot/grub.cfg"
  "$net0/"
  "$term0"
  "$cdrom0/autorun.inf"
  "$mouse0"
  "$logs"
  "$pci0/0000"
  */
  
  int disk_id = tree_open(&root, "$pci0");
  
  if (disk_id >= 0) {
    size_t count = 256; // device_feature(disk_id, FEATURE_PAGE_SIZE);
    
    uint8_t buffer[count];
    device_read(disk_id, buffer, count);
    
    for (i = 0; i < count; i++) {
      log(LOG_INFO, "%02X", buffer[i]);
      
      if (i % 16 == 15) {
        log(LOG_INFO, "\n");
      }
    }
  }
  
  const char *text = "Hello, world!\n";
  
  int console_id = tree_open(&root, "$term0");
  device_write(console_id, text, strlen(text));
  
  /* It's not like we can do much more here either... */
  for (;;);
}
