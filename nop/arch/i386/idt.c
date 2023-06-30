#ifdef __I386__

#include <nop/arch/i386/asm.h>
#include <nop/arch/i386/idt.h>
#include <nop/event.h>
#include <digits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

idt_entry_t idt_entries[256];

static void idt_pic_on(void) {
  outb(0x00, 0x0021);
  outb(0x00, 0x00A1);
  
  outb(0x35, 0x0040);
  outb(0x12, 0x0040);
}

static void idt_pic_off(void) {
  outb(0xFF, 0x0021);
  outb(0xFF, 0x00A1);
}

void idt_handle(idt_frame_t *frame, uint32_t id) {
  event_t event;
  event.data = NULL;
  
  strcpy(event.name, "int");
  ulong_to_str(id, 10, event.name + 3, EVENT_NAME_LENGTH - 3);
  
  event_trigger(event);
}

void idt_task(void) {
  /* PIC relocation. */
  outb(0x11, 0x0020);
  io_wait();
  outb(0x11, 0x00A0);
  io_wait();
  outb(0x20, 0x0021);
  io_wait();
  outb(0x28, 0x00A1);
  io_wait();
  outb(0x04, 0x0021);
  io_wait();
  outb(0x02, 0x00A1);
  io_wait();
  outb(0x01, 0x0021);
  io_wait();
  outb(0x01, 0x00A1);
  io_wait();
  
  /* PIC (and PIT). */
  idt_pic_on();
  
  size_t i;
  
  for (i = 0; i < 256; i++) {
    idt_entries[i] = (idt_entry_t){
      .offset_low = (uint16_t)(idt_handles[i] >> 0),
      .segment = 0x0008, /* Code segment? */
      
      .gate_type = idt_int_gate_32, /* TODO: Detect trap gates. */
      .max_ring = 0, /* Kernel-only. TODO: Don't. */
      .present = 1,
      
      .offset_high = (uint16_t)(idt_handles[i] >> 16),
    };
  }
  
  lidt(idt_entries);
  sti();
}

#endif
