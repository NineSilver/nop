#include <nop/device.h>
#include <stdint.h>
#include <string.h>
#include <alloc.h>

device_t *devices = NULL;
int device_used = 0, device_total = 0;

int device_add(device_t device, int no_suffix) {
  if (device_used >= device_total) {
    devices = realloc(devices, (device_total + DEVICE_ALLOC_STEP) * sizeof(device_t));
    device_total += DEVICE_ALLOC_STEP;
  }
  
  int i;
  
  for (i = 0; i < device_total; i++) {
    if (devices[i].free) {
      devices[i] = device;
      devices[i].free = 0;
      
      if (!no_suffix) {
        int length = strlen(devices[i].name);
        /* TODO: Determine next suffix and add it. */
      }
      
      device_used++;
      return i;
    }
  }
  
  return -1;
}

int device_find(const char *name) {
  int i;
  
  for (i = 0; i < device_total; i++) {
    if (!devices[i].free && !strcmp(devices[i].name, name)) {
      return i;
    }
  }
  
  return -1;
}

void device_free(int id) {
  if (id < 0 || devices[id].free) {
    return;
  }
  
  devices[id].free = 1;
  device_used--;
}

int device_feature(int id, int feature) {
  return devices[id].feature(devices + id, feature);
}

void device_commit(int id) {
  devices[id].commit(devices + id);
}

size_t device_write(int id, void *buffer, size_t size) {
  return devices[id].write(devices + id, buffer, size);
}

size_t device_read(int id, void *buffer, size_t size) {
  return devices[id].read(devices + id, buffer, size);
}

void device_seek(int id, ssize_t offset, int seek_mode) {
  return devices[id].seek(devices + id, offset, seek_mode);
}

size_t device_tell(int id) {
  return devices[id].tell(devices + id);
}

void device_trim(int id) {
  devices[id].trim(devices + id);
}
