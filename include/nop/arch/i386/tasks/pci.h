#ifndef __NOP_ARCH_I386_TASKS_PCI_H__
#define __NOP_ARCH_I386_TASKS_PCI_H__

#include <nop/start.h>
#include <stdint.h>

/* This is *not* a PCI enumerator, this only provides a
   means for the platform-agnostic PCI enumerator to access
   and write to the whole configuration space. */

extern const start_task_t pci_start_task;

void pci_task(void);

#endif
