#include <nop/tasks/ahci.h>
#include <nop/start.h>

const start_task_t ahci_start_task = (start_task_t){
  .handle = ahci_task,
  .done = 0,
};

void ahci_task(void) {
  
}
