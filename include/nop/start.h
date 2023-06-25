#ifndef __NOP_START_H__
#define __NOP_START_H__

#include <stdint.h>

typedef struct start_block_t start_block_t;
typedef struct start_task_t start_task_t;

struct start_block_t {
  void *start;
  size_t size;
};

struct start_task_t {
  void (*func)(void);
};

void start(start_block_t *blocks, int block_count, start_task_t *tasks, int task_count);

#endif
