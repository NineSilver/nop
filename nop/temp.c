#include <nop/device.h>
#include <nop/temp.h>
#include <stdint.h>
#include <alloc.h>

static int temp_device_feature(device_t *device, int feature) {
  if (feature == FEATURE_PRESENT || feature == FEATURE_WRITE || feature == FEATURE_READ || feature == FEATURE_SEEK || FEATURE_PAGE_SIZE) {
    return 1;
  }
  
  return 0;
}

static void temp_device_commit(device_t *device) {
  return;
}

static size_t temp_device_write(device_t *device, const void *ptr, size_t n) {
  temp_t *temp = device->data;
  
  if (n > temp->size - temp->offset) {
    n = temp->size - temp->offset;
  }
  
  memcpy(temp->data + temp->offset, ptr, n);
  temp->offset += n;
  
  return n;
}

static size_t temp_device_read(device_t *device, void *ptr, size_t n) {
  temp_t *temp = device->data;
  
  if (n > temp->size - temp->offset) {
    n = temp->size - temp->offset;
  }
  
  memcpy(ptr, temp->data + temp->offset, n);
  temp->offset += n;
  
  return n;
}

static void temp_device_seek(device_t *device, ssize_t offset, int seek_mode) {
  temp_t *temp = device->data;
  ssize_t current = temp->offset; /* Signed due to negative wrapping. */
  
  if (seek_mode == SEEK_SET) {
    current = offset;
  } else if (seek_mode == SEEK_END) {
    current = offset + temp->size;
  } else if (seek_mode == SEEK_CUR) {
    current += offset;
  }
  
  if (current < 0) {
    current = 0;
  } else if (current > temp->size) {
    current = temp->size;
  }
  
  temp->offset = current;
}

static size_t temp_device_tell(device_t *device) {
  temp_t *temp = device->data;
  return temp->offset;
}

static void temp_device_trim(device_t *device) {
  return;
}

int temp_alloc(const char *name, size_t n) {
  temp_t *temp = malloc(n + sizeof(temp_t));
  
  temp->size = n;
  temp->offset = 0;
  
  device_t device = (device_t){
    .is_public = 0,
    
    .data = temp,
    .free = 0,
    
    .feature = temp_device_feature,
    .commit = temp_device_commit,
    .write = temp_device_write,
    .read = temp_device_read,
    .seek = temp_device_seek,
    .tell = temp_device_tell,
    .trim = temp_device_trim,
  };
  
  strcpy(device.name, name);
  return device_add(device, 1);
}

void temp_free(int id) {
  free(devices[id].data);
  device_free(id);
}
