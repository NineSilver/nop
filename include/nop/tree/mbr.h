#ifndef __NOP_TREE_MBR_H__
#define __NOP_TREE_MBR_H__

#include <nop/tree.h>
#include <stdint.h>

typedef struct mbr_entry_t mbr_entry_t;
typedef struct mbr_table_t mbr_table_t;

typedef struct mbr_t mbr_t;

struct mbr_entry_t {
  uint8_t flags;
  uint8_t chs_start[3];
  
  uint8_t type;
  uint8_t chs_end[3];
  
  uint32_t start;
  uint32_t count;
} __attribute__((packed));

struct mbr_table_t {
  uint8_t boot_code[440];
  
  uint32_t udid;
  uint16_t zero;
  
  mbr_entry_t entries[4];
  
  uint16_t signature;
} __attribute__((packed));

struct mbr_t {
  int id;
  uint64_t offset;
  
  uint64_t start, count;
  uint8_t type;
};

int mbr_init(tree_t *tree, int id);

#endif
