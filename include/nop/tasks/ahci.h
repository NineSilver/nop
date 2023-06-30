#ifndef __NOP_TASKS_AHCI_H__
#define __NOP_TASKS_AHCI_H__

#include <nop/start.h>

extern const start_task_t ahci_start_task;

void ahci_task(void);

#endif
