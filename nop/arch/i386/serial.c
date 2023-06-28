#ifdef __I386__

#include <nop/arch/i386/serial.h>
#include <nop/arch/i386/asm.h>
#include <nop/device.h>
#include <stdint.h>

int serial_write_ready(uint16_t port) {
  return inb(port + 5) & 0x20;
}
 
int serial_read_ready(uint16_t port) {
  return inb(port + 5) & 0x01;
}

void serial_write(uint16_t port, uint8_t value) {
  while (!serial_write_ready(port));
  outb(value, port + 0);
}

uint8_t serial_read(uint16_t port) {
  while (!serial_read_ready(port));
  return inb(port + 0);
}

static int serial_device_feature(device_t *device, int feature) {
  if (feature == FEATURE_PRESENT || feature == FEATURE_WRITE || feature == FEATURE_READ || feature == FEATURE_PAGE_SIZE) {
    return 1;
  }
  
  return 0;
}

static void serial_device_commit(device_t *device) {
  return;
}

static size_t serial_device_write(device_t *device, const void *ptr, size_t n) {
  size_t i;
  
  for (i = 0; i < n; i++) {
    serial_write((size_t)(device->data), *((uint8_t *)(ptr++)));
  }
  
  return n;
}

static size_t serial_device_read(device_t *device, void *ptr, size_t n) {
  size_t i;
  
  for (i = 0; i < n; i++) {
    *((uint8_t *)(ptr++)) = serial_read((size_t)(device->data));
  }
  
  return n;
}

static void serial_device_seek(device_t *device, ssize_t offset, int seek_mode) {
  return;
}

static size_t serial_device_tell(device_t *device) {
  return 0;
}

static void serial_device_trim(device_t *device) {
  return;
}

void serial_task(void) {
  const uint16_t ports[] = {0x03F8, 0x02F8, 0x03E8, 0x02E8, 0x05F8, 0x04F8, 0x05E8, 0x04E8};
  size_t i;
  
  for (i = 0; i < 8; i++) {
    /* The provided port list may not be reliable, according to wiki.osdev.org. */
    uint16_t port = ports[i];
    
    outb(0x00, port + 1);
    outb(0x80, port + 3);
    outb(0x03, port + 0);
    outb(0x00, port + 1);
    outb(0x03, port + 3);
    outb(0xC7, port + 2);
    outb(0x0B, port + 4);
    
    /* Set loopback mode and test the device. */
    
    outb(0x1E, port + 4);
    outb(0xAE, port + 0);
  
    if (inb(port + 0) != 0xAE) {
      /* If it's not present, just continue testing. */
      continue;
    }
    
    /* Set it back to normal mode. */
    outb(0x0F, port + 4);
    
    /* Add the device. */
    
    device_add((device_t){
      .name = "term",
      .is_public = 1,
      
      .data = (void *)(port),
      .free = 0,
      
      .feature = serial_device_feature,
      .commit = serial_device_commit,
      .write = serial_device_write,
      .read = serial_device_read,
      .seek = serial_device_seek,
      .tell = serial_device_tell,
      .trim = serial_device_trim,
    }, 0);
  }
  
  return 0;
}

#endif
