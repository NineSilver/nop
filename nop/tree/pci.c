#include <nop/tree/pci.h>
#include <nop/device.h>
#include <nop/temp.h>
#include <nop/tree.h>
#include <nop/log.h>
#include <digits.h>
#include <alloc.h>
#include <nop.h>

static void pci_free(tree_t *tree) {
  free(tree->data);
}

static int pci_open(tree_t *tree, const char *path) {
  return -1; /* TODO */
}

static int pci_list(tree_t *tree, const char *path) {
  pci_t *pci = tree->data;
  int list = temp_alloc(pci->device_count * sizeof(list_t));
  
  list_t entry;
  size_t i;
  
  for (i = 0; i < pci->device_count; i++) {
    ulong_to_str(pci->devices[i], 16, entry.name, NAME_LENGTH);
    device_write(list, &entry, sizeof(list_t));
  }
  
  return list;
}

static int pci_delete(tree_t *tree, const char *path) {
  return 0;
}

static int pci_close(tree_t *tree, int id) {
  device_free(id);
  return 1;
}

static void pci_push(pci_t *pci, uint16_t vendor_id, uint16_t device_id, uint16_t addr) {
  pci->device_count++; /* TODO */
}

static void pci_test(pci_t *pci, uint16_t addr) {
  addr &= 0xFFF8; /* Masking out the 'func' bits, as we are checking them manually. */
  int i;
  
  for (i = 0; i < 8; i++) {
    size_t offset = ((size_t)(addr) << 8);
    
    uint16_t vendor_id, device_id;
    device_seek(pci->id, offset + 0x00, SEEK_SET);
    
    device_read(pci->id, &vendor_id, sizeof(uint16_t));
    device_read(pci->id, &device_id, sizeof(uint16_t));
    
    if (vendor_id == 0xFFFF) {
      continue;
    }
    
    pci_push(pci, vendor_id, device_id, addr);
    
    if (!i) {
      uint8_t header_type;
      device_seek(pci->id, offset + 0x0E, SEEK_SET);
      
      device_read(pci->id, &header_type, sizeof(uint8_t));
      
      if (!(header_type & 0x80)) {
        break;
      }
    }
    
    addr++;
  }
}

int pci_init(tree_t *tree, int id) {
  if (id < 0) {
    log(LOG_DEBUG, "[pci] Cannot create a 'pci' tree at 0x%08X, as it does not have a device attached.\n", tree);
    return 0;
  }
  
  tree->data = malloc(sizeof(pci_t));
  pci_t *pci = tree->data;
  
  pci->device_count = 0;
  pci->id = id;
  
  size_t i;
  
  for (i = 0; i < 8192; i++) {
    pci_test(pci, i << 3);
  }
  
  tree->free = pci_free;
  
  tree->open = pci_open;
  tree->list = pci_list;
  tree->delete = pci_delete;
  tree->close = pci_close;
  
  log(LOG_INFO, "[pci] Created 'pci' tree at 0x%08X, attached to device %d.\n", tree, id);
  log(LOG_INFO, "  => Found %u PCI devices.\n", pci->device_count);
  
  return 1;
}
