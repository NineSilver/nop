#include <stdint.h>
#include <alloc.h>

static void *(*block_alloc)(size_t *);
static void (*block_free)(void *, size_t);

void alloc_init(void *(*_block_alloc)(size_t *), void (*_block_free)(void *, size_t)) {
  block_alloc = _block_alloc;
  block_free = _block_free;
}

size_t alloc_usage(void) {
  /* TODO */
}

void *malloc(size_t n) {
  /* TODO */
}

void *realloc(void *ptr, size_t n) {
  /* TODO */
}

void free(void *ptr) {
  /* TODO */
}
