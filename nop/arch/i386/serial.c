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
   outb(COM_1_PORT + 0, chr);
}

uint8_t read_serial() {
   while (serial_received() == 0);
   return inb(COM_1_PORT + 0);
}

int init_serial(void) {
  outb(COM_1_PORT + 1, 0x00);
  outb(COM_1_PORT + 3, 0x80);
  outb(COM_1_PORT + 0, 0x03);
  outb(COM_1_PORT + 1, 0x00);
  outb(COM_1_PORT + 3, 0x03);
  outb(COM_1_PORT + 2, 0xC7);
  outb(COM_1_PORT + 4, 0x0B);
  
  /* Set loopback mode and test the device. */
  outb(COM_1_PORT + 4, 0x1E);
  outb(COM_1_PORT + 0, 0xAE);

  if (inb(COM_1_PORT + 0) != 0xAE) {
    return 1;
  }

  outb(COM_1_PORT + 4, 0x0F); /* Set it to normal mode. */
  return 0;
}