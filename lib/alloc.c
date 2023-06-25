#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <alloc.h>
#include <ssize.h>

/* Use a block system, with each block containing a header-data
   structure repeated all across it, and with blocks being linked
   to their next as in a linked list. */

typedef struct header_t header_t;
typedef struct block_t block_t;

struct header_t {
  /* Positive for free, negative for used. */
  ssize_t size;
  uint8_t data[];
};

struct block_t {
  block_t *next;
  size_t size;
  
  uint8_t data[];
};

static void *(*page_alloc)(size_t *);
static void (*page_free)(void *, size_t);

static block_t *block_root = NULL;
static size_t block_min = 0;

void alloc_init(void *(*_page_alloc)(size_t *), void (*_page_free)(void *, size_t), size_t _block_min) {
  page_alloc = _page_alloc;
  page_free = _page_free;
  
  block_min = _block_min;
}

void alloc_tidy(void) {
  /* TODO: Defragmentate. */
  /* TODO: Move blocks with more free space closer to the root. */
}

void alloc_block(size_t n) {
  n += sizeof(block_t) + sizeof(header_t);
  block_t *block = page_alloc(&n);
  
  block->next = block_root;
  block->size = n - sizeof(block_t);
  
  block_root = block;
  
  header_t *header = (header_t *)(block->data);
  header->size = n - sizeof(block_t) - sizeof(header_t);
}

void *malloc_once(size_t n) {
  block_t *block = block_root;
  
  while (block) {
    uint8_t *data = block->data;
    
    while (data - block->data < block->size) {
      header_t *header = (header_t *)(data);
      
      if (header->size >= n) {
        if (header->size <= n + sizeof(header_t)) {
          /* Just allocate it entirely! */
          header->size = -header->size;
        } else {
          /* Nah, it's time to split it up... */
          
          header_t *next_header = (header_t *)(data + sizeof(header_t) + n);
          next_header->size = header->size - sizeof(header_t) - n;
          
          header->size = -n;
        }
        
        return data + sizeof(header_t);
      }
      
      data += sizeof(header_t) + header->size;
    }
    
    block = block->next;
  }
  
  return NULL;
}

void *malloc(size_t n) {
  void *ptr = malloc_once(n);
  
  if (ptr) {
    return ptr;
  }
  
  /* Fine, maybe it was way too fragmented... */
  
  alloc_tidy();
  ptr = malloc_once(n);
  
  if (ptr) {
    return ptr;
  }
  
  /* If it's still failing, there might not be a big enough
     chunk of memory free for us to use, so let's get another
     block! */
  
  /* Give an extra 25% for cuteness :D */
  size_t size = n + (n >> 2); 
  
  if (size < block_min) {
    /* Stick to our size minimum. */
    size = block_min;
  }
  
  alloc_block(size);
  ptr = malloc_once(n);
  
  if (ptr) {
    return ptr;
  }
  
  /* Ok, then for sure there wasn't enough free memory... */
  return NULL;
}

void *realloc(void *ptr, size_t n) {
  /* This is a really hacky way to implement realloc(), but it
     works. */
  
  void *new_ptr = malloc(n);
  
  if (ptr) {
    header_t *header = (header_t *)(ptr - sizeof(header_t));
    
    if (header->size < 0) {
      memcpy(new_ptr, ptr, n < -header->size ? n : -header->size);
      free(ptr);
    }
  }
  
  return new_ptr;
}

void free(void *ptr) {
  if (!ptr) {
    /* We do not accept NULL pointers over here, kiddo. */
    return;
  }
  
  /* Lazily free it, make tidying up someone else's problem. */
  header_t *header = (header_t *)(ptr - sizeof(header_t));
  
  if (header->size < 0) {
    header->size = -header->size;
  }
}
