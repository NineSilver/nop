#ifndef __NOP_DEVICE_H__
#define __NOP_DEVICE_H__

#include <stdint.h>

#define DEVICE_NAME_LENGTH 15

/* 1 if present, 0 otherwise. */
#define FEATURE_PRESENT 0

/* 1 if writable, readable, seekable or trimmable device, 0 otherwise. */
#define FEATURE_WRITE 1
#define FEATURE_READ  2
#define FEATURE_BLOCK 3
#define FEATURE_TRIM  4

/* Page alignment size, 1 means no alignment at all. */
#define FEATURE_PAGE_SIZE 5

/* Unix-like seek modes. */
#define SEEK_SET 0
#define SEEK_END 1
#define SEEK_CUR 2

typedef struct device_t device_t;

struct device_t {
  char name[DEVICE_NAME_LENGTH + 1];
  
  void *data;
  int free;
  
  int (*feature)(device_t *device, int feature);
  
  size_t (*write)(device_t *device, void *buffer, size_t size);
  size_t (*read)(device_t *device, void *buffer, size_t size);
  void   (*seek)(device_t *device, ssize_t offset, int seek_mode);
  size_t (*tell)(device_t *device);
  void   (*trim)(device_t *device);
};

extern device_t *devices;
extern int device_used, device_total;

device_t *device_add(device_t device, int no_suffix);
device_t *device_find(const char *name);
void      device_remove(device_t *device);

#endif
