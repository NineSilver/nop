#ifndef __NOP_ARCH_I386_TASKS_PCI_H__
#define __NOP_ARCH_I386_TASKS_PCI_H__

#include <nop/start.h>
#include <stdint.h>

/* This is *not* a PCI enumerator, this only provides a
   means for the platform-agnostic PCI enumerator to access
   and write to the whole configuration space. */

extern const start_task_t pci_start_task;

uint32_t pci_dword(uint16_t addr, uint8_t offset);
uint16_t pci_word(uint16_t addr, uint8_t offset);
uint8_t  pci_byte(uint16_t addr, uint8_t offset);

void pci_task(void);

#endif
