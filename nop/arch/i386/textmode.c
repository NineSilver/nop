#include <nop/arch/i386/textmode.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

uint16_t *textmode_ptr;
int textmode_width, textmode_height;

int textmode_x, textmode_y;
uint16_t textmode_color;

void textmode_init(void *ptr, int width, int height, uint16_t color) {
  textmode_ptr = ptr;
  
  textmode_width = width;
  textmode_height = height;
  
  textmode_x = 0;
  textmode_y = 0;
  
  textmode_color = color;
}

void textmode_putchar(char chr) {
  if (textmode_x >= textmode_width || chr == '\n') {
    textmode_x = 0;
    textmode_y++;
    
    if (textmode_y >= textmode_height) {
      textmode_scroll();
      textmode_y--;
    }
    
    if (chr == '\n') {
      return;
    }
  }
  
  textmode_ptr[textmode_x++ + textmode_y * textmode_width] = (uint16_t)(chr) | textmode_color;
}

void textmode_scroll(void) {
  memmove(textmode_ptr, textmode_ptr + textmode_width, textmode_width * (textmode_height - 1) * sizeof(uint16_t));
  size_t i;
  
  for (i = 0; i < textmode_width; i++) {
    textmode_ptr[i + textmode_width * (textmode_height - 1)] = textmode_color;
  }
}

void textmode_write(const char *str, size_t n) {
  while (n--) {
    textmode_putchar(*(str++));
  }
}

void textmode_task(void);
