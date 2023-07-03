#include <nop/tasks/ahci.h>
#include <nop/device.h>
#include <nop/start.h>
#include <nop/page.h>
#include <nop/tree.h>
#include <nop/log.h>
#include <stdint.h>
#include <string.h>
#include <alloc.h>
#include <nop.h>

const start_task_t ahci_start_task = (start_task_t){
  .handle = ahci_task,
  .done = 0,
};

static void ahci_start_engine(hba_port_t *port) {
  while (port->cmd & 0x8000);
  
  port->cmd |= 0x0010;
  port->cmd |= 0x0001; 
}

static void ahci_stop_engine(hba_port_t *port) {
  port->cmd &= ~0xFFFFFFFE;
  port->cmd &= ~0xFFFFFFEF;
  
  while (port->cmd & 0xC000);
}

static int ahci_find_slot(hba_port_t *port) {
  uint32_t slots = (port->sact | port->ci);
  int i;
  
  for (i = 0; i < 32; i++) {
    if (!(slots & 1)) {
      return i;
    }
    
    slots >>= 1;
  }
  
  return -1;
}

static int ahci_sector_read(hba_port_t *port, uint64_t lba, void *ptr, size_t count) {
  port->is = 0xFFFFFFFF;
  int slot = ahci_find_slot(port);
  
  if (slot < 0) {
    return 0;
  }
  
  size_t header_int = (size_t)((uint64_t)(port->clb) | ((uint64_t)(port->clbu) << 32));
  hba_cmd_header_t *header = (hba_cmd_header_t *)(header_int) + slot;
  
  header->cfl = sizeof(fis_h2d_t) / sizeof(uint32_t); /* Command FIS size. */
  header->w = 0;                                      /* 0 = Device to host. */
  header->prdtl = (uint16_t)((count - 1) >> 4) + 1;   /* PRDT entry count, one per 16 sectors plus one for the remaining. */
  
  size_t table_int = (size_t)((uint64_t)(header->ctba) | ((uint64_t)(header->ctbau) << 32));
  hba_cmd_table_t *table = (hba_cmd_table_t *)(table_int);
  
  memset(table, 0, sizeof(hba_cmd_table_t) + header->prdtl * sizeof(hba_prdt_t));
  
  uint64_t ptr_64 = (size_t)(ptr);
  size_t i;
  
  for (i = 0; i < header->prdtl - 1; i++) {
    table->prdt_entry[i].dba = (uint32_t)(ptr_64 >> 0);
    table->prdt_entry[i].dbau = (uint32_t)(ptr_64 >> 32);
    
    table->prdt_entry[i].dbc = 8191;
    ptr += 8192, count -= 16;
  }
  
  table->prdt_entry[i].dba = (uint32_t)(ptr_64 >> 0);
  table->prdt_entry[i].dbau = (uint32_t)(ptr_64 >> 32);
  
  table->prdt_entry[i].dbc = (count << 9) - 1;
  fis_h2d_t *fis = (fis_h2d_t *)(&table->cfis);
  
  fis->fis_type = FIS_TYPE_H2D;
  fis->c = 1;
  fis->command = ATA_CMD_READ_DMA_EXT;
  
  fis->device = (1 << 6);
  
  fis->lba_low = (uint32_t)(lba >> 0);
  fis->lba_high = (uint32_t)(lba >> 24);
  
  fis->count = (uint16_t)(count);
  
  /* Give it enough time... */
  size_t spin = (1 << 22);
  
  while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin) {
    spin--;
  }
  
  if (!spin) {
    log(LOG_ERROR, "[ahci] Port 0x%P is hung.\n", port);
    return 0;
  }
  
  port->ci |= (1 << slot);
  
  while (port->ci & (1 << slot)) {
    if (port->is & HBA_PXIS_TFES) {
      log(LOG_ERROR, "[ahci] Failed to read sector on port 0x%P.", port);
      return 0;
    }
  }
  
  if (port->is & HBA_PXIS_TFES) {
    log(LOG_ERROR, "[ahci] Failed to read sector on port 0x%P.", port);
    return 0;
  }
 
  return 1;
}

static int ahci_device_feature(device_t *device, int feature) {
  if (feature == FEATURE_PRESENT || feature == FEATURE_WRITE || feature == FEATURE_READ || feature == FEATURE_SEEK) {
    return 1;
  }
  
  if (feature == FEATURE_PAGE_SIZE) {
    return 512;
  }
  
  return 0;
}

static void ahci_device_commit(device_t *device) {
  return;
}

static size_t ahci_device_write(device_t *device, const void *ptr, size_t n) {
  return 0;
}

static size_t ahci_device_read(device_t *device, void *ptr, size_t n) {
  ahci_t *ahci = device->data;
  size_t sector_size = (1 << ahci->sector_width);
  
  uint8_t buffer[sector_size];
  size_t read = 0;
  
  if (ahci->offset & (sector_size - 1)) {
    read += sector_size - (ahci->offset & (sector_size - 1));
    
    if (read > n) {
      read = n;
    }
    
    if (!ahci_sector_read(ahci->port, ahci->offset >> ahci->sector_width, buffer, 1)) {
      return 0;
    }
    
    memcpy(ptr, buffer + (ahci->offset & (sector_size - 1)), read);
    ptr += read, ahci->offset += read, n -= read;
  }
  
  if (!ahci_sector_read(ahci->port, ahci->offset >> ahci->sector_width, ptr, n >> ahci->sector_width)) {
    return read;
  }
  
  size_t big_read = (n - (n & (sector_size - 1)));
  
  ptr += big_read, read += big_read, ahci->offset += big_read;
  n &= (sector_size - 1);
  
  if (n) {
    if (!ahci_sector_read(ahci->port, ahci->offset >> ahci->sector_width, buffer, 1)) {
      return read;
    }
    
    memcpy(ptr, buffer, n);
    read += n;
  }
  
  return read;
}

static void ahci_device_seek(device_t *device, intmax_t offset, int seek_mode) {
  return;
}

static uintmax_t ahci_device_tell(device_t *device) {
  return 0;
}

static void ahci_device_trim(device_t *device) {
  return;
}

static void ahci_init_port(hba_port_t *port) {
  /* Stop command engine. */
  ahci_stop_engine(port);
  
  /* 1024 -> Command list. */
  /*  256 -> FIS. */
  /* 8192 -> Command table. */
  
  const size_t total_size = 9472;
  
  /* Ensure 1K alignment for every structure here. */
  uint64_t port_buffer = (size_t)(page_alloc((total_size + (PAGE_SIZE - 1)) / PAGE_SIZE, 1024));
  
  /* Clear all allocated memory. */
  memset((void *)((size_t)(port_buffer)), 0, total_size);
  
  port->clb = (uint32_t)(port_buffer);
  port->clbu = (uint32_t)(port_buffer >> 32);
  
  port->fb = (uint32_t)(port_buffer + 1024);
  port->fbu = (uint32_t)((port_buffer + 1024) >> 32);
  
  hba_cmd_header_t *header = (void *)((size_t)(port_buffer));
  size_t i;
  
  for (i = 0; i < 32; i++) {
    header[i].prdtl = 8;
    
    header[i].ctba = (uint32_t)(port_buffer + 1280 + 256 * i);
    header[i].ctbau = (uint32_t)((port_buffer + 1280 + 256 * i) >> 32);
  }
  
  /* Restart command engine. */
  ahci_start_engine(port);
  
  ahci_t *ahci = malloc(sizeof(ahci_t));
  
  ahci->port = port;
  
  ahci->sector_width = 9;
  ahci->count = 0; /* TODO */
  
  ahci->offset = 0;
  
  device_add((device_t){
    .name = "disk",
    .is_public = 1,
    
    .data = ahci,
    .free = 0,
    
    .feature = ahci_device_feature,
    .commit = ahci_device_commit,
    .write = ahci_device_write,
    .read = ahci_device_read,
    .seek = ahci_device_seek,
    .tell = ahci_device_tell,
    .trim = ahci_device_trim,
  }, 0);
}

static int ahci_init(int id) {
  device_seek(id, 0x0A, SEEK_SET);
  
  uint16_t pci_class;
  device_read(id, &pci_class, sizeof(uint16_t));
  
  if (pci_class != 0x0106) {
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
    log(LOG_DEBUG, "[ahci] AHCI controller (open at %d) in IDE mode.\n", id);
    return 0;
  }
  
  log(LOG_INFO, "[ahci] Found an AHCI controller, HBA table at 0x%P.\n", table);
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
    ahci_init_port(table->ports + i);
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
