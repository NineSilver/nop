#ifndef __NOP_ARCH_I386_TEXTMODE_H__
#define __NOP_ARCH_I386_TEXTMODE_H__

#include <stddef.h>
#include <stdint.h>

extern uint16_t *textmode_ptr;
extern int textmode_width, textmode_height;

extern int textmode_x, textmode_y;
extern uint16_t textmode_color;

void textmode_init(void *ptr, int width, int height, uint16_t color);

void textmode_putchar(char chr);
void textmode_scroll(void);

void textmode_write(const char *str, size_t n);

void textmode_task(void);

#endif
