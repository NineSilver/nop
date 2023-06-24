#ifndef __NOP_START_H__
#define __NOP_START_H__

#include <stdint.h>

typedef struct start_block_t start_block_t;

struct start_block_t {
  void *start;
  size_t size;
};

void start(const start_block_t *blocks, int block_count);

#endif
