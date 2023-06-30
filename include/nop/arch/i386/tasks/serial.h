#ifndef __NOP_ARCH_I386_TASKS_SERIAL_H__
#define __NOP_ARCH_I386_TASKS_SERIAL_H__

#include <stdint.h>

int serial_write_ready(uint16_t port); 
int serial_read_ready(uint16_t port);

void    serial_write(uint16_t port, uint8_t value);
uint8_t serial_read(uint16_t port);

void serial_task(void);

#endif
