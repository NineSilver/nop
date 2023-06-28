#ifndef __NOP_TREE_PCI_H__
#define __NOP_TREE_PCI_H__

#include <nop/tree.h>
#include <stdint.h>

#define PCI_ADDR(bus, slot, func) (((uint16_t)(bus) << 8) | ((uint16_t)(slot) << 3) | (uint16_t)(func))

typedef struct pci_device_t pci_device_t;
typedef struct pci_t pci_t;

struct pci_device_t {
  uint16_t vendor_id, device_id;
  uint16_t addr;
};

struct pci_t {
  size_t device_count;
  int id;
  
  pci_device_t devices[];
};

int pci_init(tree_t *tree, int id);

#endif