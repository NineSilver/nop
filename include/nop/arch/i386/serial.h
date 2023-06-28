#ifndef __NOP_ARCH_I386_SERIAL_H__
#define __NOP_ARCH_I386_SERIAL_H__

#include <stdint.h>

void write_serial(uint8_t chr);
uint8_t read_serial();

int init_serial(void);

#endif
