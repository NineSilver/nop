#include <nop/arch/i386/asm.h>
#include <nop/arch/i386/serial.h>

#define COM_1_PORT 0x3F8

int is_transmit_empty() {
   return inb(COM_1_PORT + 5) & 0x20;
}
 
int serial_received() {
   return inb(COM_1_PORT + 5) & 0x01;
}

void write_serial(uint8_t chr) {
   while (is_transmit_empty() == 0);
   outb(chr, COM_1_PORT + 0);
}

uint8_t read_serial() {
   while (serial_received() == 0);
   return inb(COM_1_PORT + 0);
}

int init_serial(void) {
  outb(0x00, COM_1_PORT + 1);
  outb(0x80, COM_1_PORT + 3);
  outb(0x03, COM_1_PORT + 0);
  outb(0x00, COM_1_PORT + 1);
  outb(0x03, COM_1_PORT + 3);
  outb(0xC7, COM_1_PORT + 2);
  outb(0x0B, COM_1_PORT + 4);
  
  /* Set loopback mode and test the device. */
  outb(0x1E, COM_1_PORT + 4);
  outb(0xAE, COM_1_PORT + 0);

  if (inb(COM_1_PORT + 0) != 0xAE) {
    return 1;
  }

  outb(0x0F, COM_1_PORT + 4); /* Set it to normal mode. */
  return 0;
}
