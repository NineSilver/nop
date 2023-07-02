#include <nop/device.h>
#include <nop/log.h>
#include <digits.h>
#include <stdint.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>

device_t *devices = NULL;
size_t device_used = 0, device_total = 0;

int device_add(device_t device, int no_suffix) {
  if (device_used >= device_total) {
    devices = realloc(devices, (device_total + DEVICE_ALLOC_STEP) * sizeof(device_t));
    device_total += DEVICE_ALLOC_STEP;
  }
  
  size_t i;
  
  for (i = 0; i < device_total; i++) {
    if (devices[i].free) {
      devices[i] = device;
      devices[i].free = 0;
      
      if (!no_suffix) {
        int length = strlen(devices[i].name);
        
        unsigned long suffix_value = 0;
        size_t j;
        
        for (j = 0; j < device_total; j++) {
          if (i == j) {
            continue;
          }
          
          if (!memcmp(devices[i].name, devices[j].name, length) && isdigit(devices[j].name[length])) {
            unsigned long value = str_to_ulong(10, devices[j].name + length, DEVICE_NAME_LENGTH - length);
            suffix_value = value + 1;
          }
        }
        
        ulong_to_str(suffix_value, 10, devices[i].name + length, DEVICE_NAME_LENGTH - length);
      }
      
      if (devices[i].is_public) {
        log(LOG_INFO, "[device] Added device '%s'.\n", devices[i].name);
      }
      
      device_used++;
      return i;
    }
  }
  
  return -1;
}

int device_find(const char *name) {
  size_t i;
  
  for (i = 0; i < device_total; i++) {
    if (!devices[i].free && !strncmp(devices[i].name, name, DEVICE_NAME_LENGTH)) {
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

size_t device_write(int id, const void *ptr, size_t n) {
  return devices[id].write(devices + id, ptr, n);
}

size_t device_read(int id, void *ptr, size_t n) {
  return devices[id].read(devices + id, ptr, n);
}

void device_seek(int id, intmax_t offset, int seek_mode) {
  return devices[id].seek(devices + id, offset, seek_mode);
}

uintmax_t device_tell(int id) {
  return devices[id].tell(devices + id);
}

void device_trim(int id) {
  devices[id].trim(devices + id);
}
