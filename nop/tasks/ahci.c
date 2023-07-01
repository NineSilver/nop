#include <nop/tasks/ahci.h>
#include <nop/device.h>
#include <nop/start.h>
#include <nop/page.h>
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
    
    log(LOG_DEBUG, "[ahci] PCI device (open at %d) not an AHCI controller (got class 0x%04X).\n", id, pci_class);
    return 0;
  }
  
  /* Get the table's address in BAR5, then mark it in our page allocator. */
  device_seek(id, 0x24, SEEK_SET);
  
  hba_table_t *table;
  device_read(id, &table, sizeof(void *));
  
  size_t page_count = (sizeof(hba_table_t) + (PAGE_SIZE - 1)) / PAGE_SIZE;
  size_t page_start = (size_t)(table) / PAGE_SIZE;
  
  page_mark(page_start, page_start + page_count, PAGE_USED);
  
  if (!(table->ghc & 0x80000000)) {
    /* The device isn't set up as an AHCI controller, oops. */
    
    log(LOG_DEBUG, "[ahci] AHCI controller (open at %d) in IDE mode.\n", id);
    return 0;
  }
  
  log(LOG_INFO, "[ahci] Found an AHCI controller, HBA table at 0x%08X.\n", table);
  size_t i;
  
  for (i = 0; i < 32; i++) {
    if (!(table->pi & ((uint32_t)(1) << i))) {
      continue;
    }
    
    uint32_t ssts = table->ports[i].ssts;
    
    if ((ssts & 0x0F) != 3 || ((ssts >> 8) & 0x0F) != 1) {
      continue;
    }
    
    uint32_t sig = table->ports[i].sig;
    
    if (sig == AHCI_SIG_ATAPI) {
      log(LOG_WARNING, "  => Port %u has an ATAPI device, not implemented.\n", i);
      continue;
    } else if (sig == AHCI_SIG_SEMB) {
      log(LOG_WARNING, "  => Port %u has an SEMB device, not implemented.\n", i);
      continue;
    } else if (sig == AHCI_SIG_PM) {
      log(LOG_WARNING, "  => Port %u has a port multiplier, not implemented.\n", i);
      continue;
    }
    
    log(LOG_INFO, "  => Found SATA drive at port %u.\n", i);
    ahci_init_port(table, i);
  }
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
