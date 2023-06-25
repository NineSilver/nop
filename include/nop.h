#ifndef __NOP_H__
#define __NOP_H__

/* Things actually exposed to both userspace and the kernel. */

typedef struct nop_list_t nop_list_t;

struct nop_list_t {
  char name[256];
};

#endif
