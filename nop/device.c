#include <nop/device.h>
#include <stdint.h>
#include <string.h>
#include <alloc.h>

device_t *devices = NULL;
int device_used = 0, device_total = 0;

device_t *device_add(device_t device, int no_suffix) {
  if (device_used >= device_total) {
    /* TODO: Try to increase size. */
  }
  
  for (int i = 0; i < device_total; i++) {
    if (devices[i].free) {
      devices[i] = device;
      devices[i].free = 0;
      
      if (!no_suffix) {
        int length = strlen(devices[i].name);
        /* TODO: Determine next suffix and add it. */
      }
      
      device_used++;
      return devices + i;
    }
  }
  
  return NULL;
}

device_t *device_find(const char *name) {
  for (int i = 0; i < device_total; i++) {
    if (!devices[i].free && !strcmp(devices[i].name, name)) {
      return devices + i;
    }
  }
  
  return NULL;
}

void device_remove(device_t *device) {
  if (!device || device->free) {
    return;
  }
  
  device->free = 1;
  device_used--;
}
