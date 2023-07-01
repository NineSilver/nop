#include <nop/tasks/ahci.h>
#include <nop/device.h>
#include <nop/start.h>
#include <nop/tree.h>
#include <nop/log.h>
#include <stdint.h>
#include <string.h>
#include <nop.h>

const start_task_t ahci_start_task = (start_task_t){
  .handle = ahci_task,
  .done = 0,
};

static void ahci_init_port(hba_table_t *table, int port) {
  
}

static int ahci_init(int id) {
  device_seek(id, 0x0A, SEEK_SET);
  
  uint16_t pci_class;
  device_read(id, &pci_class, sizeof(uint16_t));
  
  if (pci_class != 0x0106) {
    /* Not a Serial ATA device. */
    return 0;
  }
  
  /* Get the table's address from BAR5. */
  device_seek(id, 0x24, SEEK_SET);
  
  hba_table_t *table;
  device_read(id, &table, sizeof(void *));
  
  if (!(table->ghc & 0x80000000)) {
    /* The device's not set up as an AHCI controller, oops. */
    return 0;
  }
  
  log(LOG_INFO, "yay\n");
}

void ahci_task(void) {
  int root_list = tree_list(&root, "");
  list_t root_entry;
  
  if (root_list < 0) {
    return;
  }
  
  while (device_read(root_list, &root_entry, sizeof(list_t)) >= sizeof(list_t)) {
    if (memcmp(root_entry.name, "$pci", 4) || !isdigit(root_entry.name[4]) || !root_entry.can_list) {
      continue;
    }
    
    int pci_list = tree_list(&root, root_entry.name);
    list_t pci_entry;
    
    if (pci_list < 0) {
      continue;
    }
    
    while (device_read(pci_list, &pci_entry, sizeof(list_t)) >= sizeof(list_t)) {
      if (!pci_entry.can_open) {
        continue;
      }
      
      char path[256];
      
      strcpy(path, root_entry.name);
      strcat(path, "/");
      strcat(path, pci_entry.name);
      
      int id = tree_open(&root, path);
      
      if (id < 0) {
        continue;
      }
      
      if (!ahci_init(id)) {
        tree_close(&root, id);
      }
    }
    
    tree_close(&root, pci_list);
  }
  
  tree_close(&root, root_list);
}
