#ifndef __NOP_DEVICE_H__
#define __NOP_DEVICE_H__

#include <stddef.h>
#include <stdint.h>
#include <ssize.h>

#define DEVICE_NAME_LENGTH 15
#define DEVICE_ALLOC_STEP  8

/* 1 if present, 0 otherwise. */
#define FEATURE_PRESENT 0

/* 1 if writable, readable, seekable or trimmable device, 0 otherwise. */
#define FEATURE_WRITE 1
#define FEATURE_READ  2
#define FEATURE_SEEK  3
#define FEATURE_TRIM  4

/* Page alignment size, 1 means no alignment at all. */
#define FEATURE_PAGE_SIZE 5

/* 1 if it's not automatically commited and needs a commit() call. */
#define FEATURE_COMMIT 6

/* Unix-like seek modes. */
#define SEEK_SET 0
#define SEEK_END 1
#define SEEK_CUR 2

typedef struct device_t device_t;

struct device_t {
  char name[DEVICE_NAME_LENGTH + 1];
  int is_public;
  
  void *data;
  int free;
  
  int       (*feature)(device_t *device, int feature);
  void      (*commit)(device_t *device);
  size_t    (*write)(device_t *device, const void *ptr, size_t n);
  size_t    (*read)(device_t *device, void *ptr, size_t n);
  void      (*seek)(device_t *device, intmax_t offset, int seek_mode);
  uintmax_t (*tell)(device_t *device);
  void      (*trim)(device_t *device);
};

extern device_t *devices;
extern size_t device_used, device_total;

int  device_add(device_t device, int no_suffix);
int  device_find(const char *name);
void device_free(int id);

int       device_feature(int id, int feature);
void      device_commit(int id);
size_t    device_write(int id, const void *ptr, size_t n);
size_t    device_read(int id, void *ptr, size_t n);
void      device_seek(int id, intmax_t offset, int seek_mode);
uintmax_t device_tell(int id);
void      device_trim(int id);

#endif
