#ifdef __I386__

#include <nop/arch/i386/tasks/pci.h>
#include <nop/arch/i386/asm.h>
#include <nop/device.h>
#include <stdint.h>

const start_task_t pci_start_task = (start_task_t){
  .handle = pci_task,
  .done = 0,
};

uint32_t pci_dword(uint16_t addr, uint8_t offset) {
  offset &= 0xFC;
  
  outd(0x80000000 | ((uint32_t)(addr) << 8) | (uint32_t)(offset), 0x0CF8);
  return ind(0x0CFC);
}

uint16_t pci_word(uint16_t addr, uint8_t offset) {
  offset &= 0xFE;
  return (uint16_t)(pci_dword(addr, offset) >> ((offset & 2) << 3));
}

uint8_t pci_byte(uint16_t addr, uint8_t offset) {
  return (uint8_t)(pci_dword(addr, offset) >> ((offset & 3) << 3));
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
  return; /* Commits aren't necessary. */
}

static size_t pci_device_write(device_t *device, const void *ptr, size_t n) {
  return 0; /* TODO: Implement writes. */
}

static size_t pci_device_read(device_t *device, void *ptr, size_t n) {
  const size_t size = 0x01000000; /* 16 MiB total, 2^16 devices times 2^8 offsets. */
  size_t offset = (size_t)(device->data);
  
  if (n > size - offset) {
    n = size - offset;
  }
  
  if (offset & 3) {
    uint32_t value = pci_dword(offset >> 8, offset);
    uint8_t *bytes = (uint8_t *)(&value);
    
    while (n && (offset & 3)) {
      *((uint8_t *)(ptr)) = bytes[offset & 3];
      offset++, ptr++, n--;
    }
  }
  
  while (n >= 4) {
    *((uint32_t *)(ptr)) = pci_dword(offset >> 8, offset);
    offset += 4, ptr += 4, n -= 4;
  }
  
  if (n & 3) {
    uint32_t value = pci_dword(offset >> 8, offset);
    uint8_t *bytes = (uint8_t *)(&value);
    
    while (n) {
      *((uint8_t *)(ptr)) = bytes[offset & 3];
      offset++, ptr++, n--;
    }
  }
  
  device->data = (void *)(offset);
}

static void pci_device_seek(device_t *device, ssize_t offset, int seek_mode) {
  const size_t size = 0x01000000; /* 16 MiB total, 2^16 devices times 2^8 offsets. */
  ssize_t current = (size_t)(device->data); /* Signed due to negative wrapping. */
  
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
  
  device->data = (void *)(current);
}

static size_t pci_device_tell(device_t *device) {
  return (size_t)(device->data);
}

static void pci_device_trim(device_t *device) {
  return; /* You cannot trim a PCI confifuration space. */
}

void pci_task(void) {
  device_add((device_t){
    .name = "pci",
    .is_public = 1,
    
    .data = (void *)(0),
    .free = 0,
    
    .feature = pci_device_feature,
    .commit = pci_device_commit,
    .write = pci_device_write,
    .read = pci_device_read,
    .seek = pci_device_seek,
    .tell = pci_device_tell,
    .trim = pci_device_trim,
  }, 0);
}

#endif
