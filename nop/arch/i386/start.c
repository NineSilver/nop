#include <nop/arch/i386/multiboot2.h>
#include <nop/start.h>
#include <stddef.h>
#include <stdint.h>

extern int i386_load_start, i386_load_end, i386_bss_end;

void i386_start_c(void *tags, size_t total_size) {
  size_t i;
  
  /* Iterate through all the Multiboot2 tags. */
  
  for (i = 0; i < total_size;) {
    struct multiboot_tag *tag = tags;
    
    if (tag->type == MULTIBOOT_TAG_TYPE_END) {
      break;
    } else if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
      struct multiboot_tag_mmap *tag_mmap = tags;
      
      size_t j;
      
      for (j = 0; j < tag_mmap.size / sizeof(struct multiboot_mmap_entry); j++) {
        if (tag_mmap.entries[j].type != MULTIBOOT_MEMORY_AVAILABLE) {
          continue;
        }
        
        size_t entry_start = tag_mmap.entries[j].addr;
        size_t entry_end = tag_mmap.entries[j].addr + tag_mmap.entries[j].len;
        
        /* Real mode 1 MiB area should be kinda untouchable... */
        
        if (entry_start < 0x00100000) {
          entry_start = 0x00100000;
        }
        
        /* Kludgey way to prevent marking the kernel as free. */
        
        if (entry_start >= i386_load_start && entry_start < i386_bss_end) {
          entry_start = i386_bss_end;
        }
        
        if (entry_start >= i386_load_start && entry_end < i386_bss_end) {
          entry_end = i386_load_start;
        }
        
        /* If while doing that we end up mutilating the free space, just fuck it. */
        
        if (entry_end <= entry_start) {
          continue;
        }W
        
        /* TODO: Add to some random array to pass in to the actual kernel entry. */
      }
    }
    
    i = ((i + tag->size + (MULTIBOOT_TAG_ALIGN - 1)) & ~(MULTIBOOT_TAG_ALIGN - 1));
  }
  
  const char *text = "Hello, world!";
  
  for (i = 0; text[i]; i++) {
    ((uint16_t *)(0x000B8000))[i] = (uint16_t)(text[i]) | 0xF000;
  }
  
  start(NULL, 0, NULL, 0);
  
  for (;;);
}
