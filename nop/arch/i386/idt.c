#ifdef __I386__

#include <nop/arch/i386/idt.h>
#include <stdint.h>

idt_entry_t idt_entries[256];

void idt_task(void) {
  /* TODO: Exist. */
}

#endif
