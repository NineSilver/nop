#ifndef __NOP_ARCH_I386_TASKS_IDT_H__
#define __NOP_ARCH_I386_TASKS_IDT_H__

#include <stddef.h>
#include <stdint.h>

typedef struct idt_entry_t idt_entry_t;
typedef struct idt_frame_t idt_frame_t;

enum {
  idt_task_gate    = 0x05,
  idt_int_gate_16  = 0x06,
  idt_trap_gate_16 = 0x07,
  idt_int_gate_32  = 0x0E,
  idt_trap_gate_32 = 0x0F,
};

struct idt_entry_t {
  uint16_t offset_low;
  uint16_t segment;
  
  uint8_t reserved;
  
  uint8_t gate_type: 4;
  uint8_t zero: 1;
  uint8_t max_ring: 2; /* Highest ring that could 'int 0xNN' this entry (inclusive). */
  uint8_t present: 1;
  
  uint16_t offset_high;
} __attribute__((packed));

struct idt_frame_t {
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t eip, cs, eflags;
} __attribute__((packed));

extern idt_entry_t idt_entries[];
extern size_t idt_handles[];

void idt_handle(idt_frame_t *frame, uint32_t id);
void idt_task(void);

#endif
