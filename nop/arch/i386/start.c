#include <nop/arch/i386/multiboot2.h>
#include <nop/start.h>
#include <stddef.h>
#include <stdint.h>

void i386_start_c(void *tags, size_t total_size) {
  const char *text = "Hello, world!";
  int i;
  
  for (i = 0; text[i]; i++) {
    ((uint16_t *)(0x000B8000))[i] = (uint16_t)(text[i]) | 0xF000;
  }
  
  start(NULL, 0, NULL, 0);
  
  for (;;);
}
