#ifdef __I386__

#include <nop/arch/i386/multiboot2.h>
#include <nop/arch/i386/serial.h>
#include <nop/arch/i386/text.h>
#include <nop/arch/i386/pci.h>
#include <nop/start.h>
#include <nop/log.h>
#include <alloca.h>
#include <stddef.h>
#include <stdint.h>

extern int i386_load_start, i386_load_end, i386_bss_start, i386_bss_end;

/* Our actual entry point, right after the Multiboot2 header and
   assembly blob. */

void i386_start_c(void *tags) {
  start_block_t *blocks = NULL;
  size_t block_count = 0;
  
  size_t i;
  
  /* Start 0x000B8000 text mode, and use if for early logging (TODO: Detect!). */
  
  text_init((void *)(0x000B8000), 80, 25, 0x0F00);
  log_early(text_write);
  
  log(LOG_DEBUG, "[i386] .text/.data => (0x%08X - 0x%08X)\n", &i386_load_start, &i386_load_end);
  log(LOG_DEBUG, "[i386] .bss => (0x%08X - 0x%08X)\n", &i386_bss_start, &i386_bss_end);
  
  /* Iterate through all the Multiboot2 tags. */
  
  log(LOG_DEBUG, "[i386] Multiboot2 data found at 0x%08X.\n", tags);
  
  size_t total_size = *((uint32_t *)(tags));
  tags += 8;
  
  log(LOG_DEBUG, "[i386] Iterating through all Multiboot2 tags...\n");
  
  for (i = 0; i < total_size;) {
    struct multiboot_tag *tag = (void *)(tags + i);
    
    log(LOG_DEBUG, "- Multiboot2 tag of type %u (i = %u).\n", tag->type, i);
    
    if (tag->type == MULTIBOOT_TAG_TYPE_END) {
      break;
    } else if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
      struct multiboot_tag_mmap *tag_mmap = (void *)(tags + i);
      
      size_t entry_count = (tag_mmap->size - sizeof(struct multiboot_tag_mmap)) / sizeof(struct multiboot_mmap_entry);
      size_t j;
      
      log(LOG_DEBUG, "  => Memory map tag with %u entries.\n", entry_count);
      
      blocks = (void *)(&i386_bss_start);
      block_count = 0;
      
      start_block_t entries[entry_count];
      
      for (j = 0; j < entry_count; j++) {
        if (tag_mmap->entries[j].type != MULTIBOOT_MEMORY_AVAILABLE) {
          continue;
        }
        
        size_t entry_start = tag_mmap->entries[j].addr;
        size_t entry_end = tag_mmap->entries[j].addr + tag_mmap->entries[j].len;
        
        /* Real mode 1 MiB area should be kinda untouchable... */
        
        if (entry_start < 0x00100000) {
          entry_start = 0x00100000;
        }
        
        /* Kludgey way to prevent marking the kernel as free. */
        
        if (entry_start >= (size_t)(&i386_load_start) && entry_start < (size_t)(&i386_bss_end)) {
          entry_start = (size_t)(&i386_bss_end);
        }
        
        if (entry_start >= (size_t)(&i386_load_start) && entry_end < (size_t)(&i386_bss_end)) {
          entry_end = (size_t)(&i386_load_start);
        }
        
        /* If while doing that we end up mutilating the free space, just fuck it. */
        
        if (entry_end <= entry_start) {
          continue;
        }
        
        /* Add to some random array to pass in to the actual kernel entry. */
        
        blocks[block_count++] = (start_block_t){
          .start = (void *)(entry_start),
          .size = entry_end - entry_start,
        };
      }
    }
    
    i += tag->size + (MULTIBOOT_TAG_ALIGN - 1);
    i &= ~(MULTIBOOT_TAG_ALIGN - 1);
  }
  
  start_task_t tasks[] = {
    (start_task_t){
      .handle = text_task,
      .needs = "",
      
      .done = 0,
    },
    
    (start_task_t){
      .handle = serial_task,
      .needs = "",
      
      .done = 0,
    },
    
    (start_task_t){
      .handle = pci_task,
      .needs = "",
      
      .done = 0,
    },
  };
  
  start(blocks, block_count, tasks, sizeof(tasks) / sizeof(start_task_t));
}

#endif
