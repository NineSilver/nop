#ifndef __NOP_START_H__
#define __NOP_START_H__

#include <stddef.h>
#include <stdint.h>

typedef struct start_block_t start_block_t;
typedef struct start_task_t start_task_t;

struct start_block_t {
  void *start;
  size_t size;
};

struct start_task_t {
  void (*handle)(void);
  int done;
};

void start(start_block_t *blocks, size_t block_count, start_task_t *tasks, size_t task_count);

#endif
