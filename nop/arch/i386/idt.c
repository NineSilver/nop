#ifdef __I386__

#include <nop/arch/i386/asm.h>
#include <nop/arch/i386/idt.h>
#include <stddef.h>
#include <stdint.h>

idt_entry_t idt_entries[256];

void idt_handle(uint32_t id, idt_frame_t *frame) {
  /* Nothing. */
}

void idt_task(void) {
  size_t i;
  
  for (i = 0; i < 256; i++) {
    idt_entries[i] = (idt_entry_t){
      .offset_low = (uint16_t)((size_t)(idt_handles[i]) >> 0),
      .segment = 0x0008, /* Code segment? */
      
      .gate_type = idt_int_gate_32, /* TODO: Detect trap gates. */
      .max_ring = 0, /* Kernel-only. TODO: Don't. */
      .present = 1,
      
      .offset_high = (uint16_t)((size_t)(idt_handles[i]) >> 16),
    };
  }
  
  lidt(idt_entries);
  sti();
}

#endif
