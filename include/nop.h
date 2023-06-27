#ifndef __NOP_H__
#define __NOP_H__

/* Things actually exposed to both userspace and the kernel. */

#define NAME_LENGTH 255

typedef struct list_t list_t;

struct list_t {
  char name[NAME_LENGTH + 1];
} __attribute__((packed));

#endif
