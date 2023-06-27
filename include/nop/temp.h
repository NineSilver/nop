#ifndef __NOP_TEMP_H__
#define __NOP_TEMP_H__

#include <stdint.h>

typedef struct temp_t temp_t;

struct temp_t {
  size_t size, offset;
  uint8_t data[];
};

int  temp_alloc(size_t n);
void temp_free(int id);

#endif
