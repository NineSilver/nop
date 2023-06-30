#ifndef __NOP_ARCH_I386_TASKS_TEXT_H__
#define __NOP_ARCH_I386_TASKS_TEXT_H__

#include <nop/start.h>
#include <stddef.h>
#include <stdint.h>

extern const start_task_t text_start_task;

extern uint16_t *text_ptr;
extern int text_width, text_height;

extern int text_x, text_y;
extern uint16_t text_color;

void text_init(void *ptr, int width, int height, uint16_t color);

void text_putchar(char chr);
void text_scroll(void);

void text_write(const char *str, size_t n);

void text_task(void);

#endif
