#include <nop/tree/pci.h>
#include <nop/device.h>
#include <nop/temp.h>
#include <nop/tree.h>
#include <nop/log.h>
#include <digits.h>
#include <stdint.h>
#include <alloc.h>
#include <nop.h>

static void pci_mount(tree_t *tree) {
  return;
}

static void pci_free(tree_t *tree) {
  free(tree->data);
}

static int pci_device_feature(device_t *device, int feature) {
  if (feature == FEATURE_PRESENT || feature == FEATURE_WRITE || feature == FEATURE_READ || feature == FEATURE_SEEK) {
    return 1;
  }
  
  if (feature == FEATURE_PAGE_SIZE) {
    return 4;
  }
  
  return 0;
}

static void pci_device_commit(device_t *device) {
  return;
}

static size_t pci_device_write(device_t *device, const void *ptr, size_t n) {
  const size_t size = 0x0100;
  pci_device_t *pci_device = device->data;
  
  if (n > size - pci_device->offset) {
    n = size - pci_device->offset;
  }
  
  device_seek(pci_device->id, pci_device->offset | ((size_t)(pci_device->addr) << 8), SEEK_SET);
  return device_write(pci_device->id, ptr, n);
}

static size_t pci_device_read(device_t *device, void *ptr, size_t n) {
  const size_t size = 0x0100;
  pci_device_t *pci_device = device->data;
  
  if (n > size - pci_device->offset) {
    n = size - pci_device->offset;
  }
  
  device_seek(pci_device->id, pci_device->offset | ((size_t)(pci_device->addr) << 8), SEEK_SET);
  return device_read(pci_device->id, ptr, n);
}

static void pci_device_seek(device_t *device, intmax_t offset, int seek_mode) {
  const size_t size = 0x0100;
  
  pci_device_t *pci_device = device->data;
  ssize_t current = pci_device->offset; /* Signed due to negative wrapping. */
  
  if (seek_mode == SEEK_SET) {
    current = offset;
  } else if (seek_mode == SEEK_END) {
    current = offset + size;
  } else if (seek_mode == SEEK_CUR) {
    current += offset;
  }
  
  if (current < 0) {
    current = 0;
  } else if (current > size) {
    current = size;
  }
  
  pci_device->offset = current;
}

static uintmax_t pci_device_tell(device_t *device) {
  pci_device_t *pci_device = device->data;
  return pci_device->offset;
}

static void pci_device_trim(device_t *device) {
  return;
}

static int pci_open(tree_t *tree, const char *path) {
  if (!(*path)) {
    return -1;
  }
  
  pci_t *pci = tree->data;
  
  pci_device_t *device = malloc(sizeof(pci_device_t));
  
  device->id = pci->id;
  device->offset = 0;
  
  device->vendor_id = str_to_ulong(16, path + 0, 4);
  device->device_id = str_to_ulong(16, path + 5, 4);
  device->addr = str_to_ulong(16, path + 10, 4);
  
  device_add((device_t){
    .name = "pci_open",
    .is_public = 0,
    
    .data = device,
    .free = 0,
    
    .feature = pci_device_feature,
    .commit = pci_device_commit,
    .write = pci_device_write,
    .read = pci_device_read,
    .seek = pci_device_seek,
    .tell = pci_device_tell,
    .trim = pci_device_trim,
  }, 1);
}

static int pci_list(tree_t *tree, const char *path) {
  if (*path) {
    return -1;
  }
  
  pci_t *pci = tree->data;
  int list = temp_alloc("pci_list", pci->device_count * sizeof(list_t));
  
  list_t entry;
  size_t i;
  
  for (i = 0; i < pci->device_count; i++) {
    ulong_to_str_align(pci->devices[i].vendor_id, 16, entry.name, 4);
    ulong_to_str_align(pci->devices[i].device_id, 16, entry.name + 5, 4);
    ulong_to_str_align(pci->devices[i].addr, 16, entry.name + 10, 4);
    
    entry.name[4] = '_';
    entry.name[9] = '_';
    
    entry.can_open = 1;
    entry.can_list = 0;
    
    device_write(list, &entry, sizeof(list_t));
  }
  
  device_seek(list, 0, SEEK_SET);
  return list;
}

static int pci_delete(tree_t *tree, const char *path) {
  return 0;
}

static int pci_close(tree_t *tree, int id) {
  if (!strcmp(devices[id].name, "pci_list")) {
    temp_free(id);
  } else {
    /* TODO */
    device_free(id);
  }
  
  return 1;
}

static pci_t *pci_push(pci_t *pci, uint16_t vendor_id, uint16_t device_id, uint16_t addr) {
  pci = realloc(pci, sizeof(pci_t) + (pci->device_count + 1) * sizeof(pci_device_t));
  
  pci->devices[pci->device_count++] = (pci_device_t){
    .id = pci->id,
    .offset = 0,
    
    .vendor_id = vendor_id,
    .device_id = device_id,
    .addr = addr,
  };
  
  return pci;
}

static pci_t *pci_test(pci_t *pci, uint16_t addr) {
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
    
    /* log(LOG_DEBUG, "%04X_%04X_%04X\n", vendor_id, device_id, addr); */
    
    pci = pci_push(pci, vendor_id, device_id, addr);
    
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
  
  return pci;
}

int pci_init(tree_t *tree, int id) {
  if (id < 0 || !device_feature(id, FEATURE_PRESENT)) {
    log(LOG_DEBUG, "[pci] Cannot mount 'pci' at 0x%08X, as it does not have a device attached.\n", tree);
    return 0;
  }
  
  if (memcmp(tree->name, "$pci", 4)) {
    /* TODO: Do this in a cleaner way, maybe don't even require this? */
    
    log(LOG_DEBUG, "[pci] Cannot mount 'pci' at 0x%08X, as it is not named '$pci*'.\n", tree);
    return 0;
  }
  
  if (!device_feature(id, FEATURE_WRITE) || !device_feature(id, FEATURE_READ) || !device_feature(id, FEATURE_SEEK)) {
    log(LOG_DEBUG, "[pci] Cannot mount 'pci' at 0x%08X, as it does not have the needed features..\n", tree);
    return 0;
  }
  
  pci_t *pci = malloc(sizeof(pci_t));
  
  pci->device_count = 0;
  pci->id = id;
  
  size_t i;
  
  for (i = 0; i < 8192; i++) {
    pci = pci_test(pci, i << 3);
  }
  
  tree->data = pci;
  
  tree->mount = pci_mount;
  tree->free = pci_free;
  
  tree->open = pci_open;
  tree->list = pci_list;
  tree->delete = pci_delete;
  tree->close = pci_close;
  
  log(LOG_INFO, "[pci] Mounted 'pci' at 0x%08X, attached to device %d.\n", tree, id);
  log(LOG_INFO, "  => Found %u PCI devices.\n", pci->device_count);
  
  return 1;
}
