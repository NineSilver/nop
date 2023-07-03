#include <nop/tree/mbr.h>
#include <nop/temp.h>
#include <nop/tree.h>
#include <stdint.h>
#include <alloc.h>
#include <nop.h>

static void mbr_mount(tree_t *tree) {
  /*
  size_t i, j;
  tree_t child;
  
  for (i = 0; i < device_total; i++) {
    if (devices[i].free || !devices[i].is_public) {
      continue;
    }
    
    int found = 0;
    
    for (j = 0; j < tree->child_count; j++) {
      if (!strcmp(tree->childs[j].name + 1, devices[i].name)) {
        found = 1;
        break;
      }
    }
    
    if (found) {
      continue;
    }
    
    strcpy(child.name + 1, devices[i].name);
    child.name[0] = '$';
    
    if (!tree_init(&child, i)) {
      continue;
    }
    
    tree->childs = realloc(tree->childs, (tree->child_count + 1) * sizeof(tree_t));
    tree->childs[tree->child_count++] = child;
  }
  */
}

static void mbr_free(tree_t *tree) {
  return; /* TODO */
}

static int mbr_open(tree_t *tree, const char *path) {
  return -1; /* TODO */
}

static int mbr_list(tree_t *tree, const char *path) {
  if (*path) {
    return -1;
  }
  
  mbr_t *mbr = tree->data;
  
  size_t i, j, count = 0;
  list_t entry;
  
  for (i = 0; i < 4; i++) {
    count += (mbr[i].id >= 0);
  }
  
  int list = temp_alloc("mbr_list", count * sizeof(list_t));
  
  for (i = 0; i < 4; i++) {
    if (mbr[i].id < 0) {
      continue;
    }
    
    ulong_to_str(i, 10, entry.name, TREE_NAME_LENGTH);
    
    entry.can_open = 1;
    entry.can_list = 0;
    
    /* TODO: Filesystem mounts. */
    
    device_write(list, &entry, sizeof(list_t));
  }
  
  device_seek(list, 0, SEEK_SET);
  return list;
}

static int mbr_delete(tree_t *tree, const char *path) {
  return 0;
}

static int mbr_close(tree_t *tree, int id) {
  return 0; /* TODO */
}

int mbr_init(tree_t *tree, int id) {
  if (id < 0 || !device_feature(id, FEATURE_PRESENT)) {
    return 0;
  }
  
  if (!device_feature(id, FEATURE_WRITE) || !device_feature(id, FEATURE_READ) || !device_feature(id, FEATURE_SEEK)) {
    return 0;
  }
  
  if (device_feature(id, FEATURE_PAGE_SIZE) != 512) {
    return 0;
  }
  
  mbr_table_t table;
  size_t i;
  
  device_seek(id, 0, SEEK_END);
  
  uint64_t count = device_tell(id);
  count >>= 9;
  
  device_seek(id, 0, SEEK_SET);
  device_read(id, &table, sizeof(mbr_table_t));
  
  /* In order to ensure it is an MBR partitioned drive, we must check that:
     1. No partitions reside outside the drive.
     2. No partitions overlap.
     3. There's at least one partition.
     4. No partition has the type 0xEE (GPT protective).
     5. No partition with LBA start != 0 has sector count 0. */
  
  mbr_t mbr[4];
  int mbr_count = 0;
  
  for (i = 0; i < 4; i++) {
    if (!table.entries[i].start) {
      if (table.entries[i].count) {
        return 0;
      }
    }
    
    if (table.entries[i].start) {
      if (!table.entries[i].count) {
        return 0;
      }
    }
    
    if (!table.entries[i].count) {
      continue;
    }
    
    if ((uint64_t)(table.entries[i].start) + (uint64_t)(table.entries[i].count) > count) {
      return 0;
    }
    
    if (table.entries[i].type == 0xEE) {
      return 0;
    }
    
    mbr[mbr_count] = (mbr_t){
      .id = id,
      .offset = 0,
      
      .start = table.entries[i].start,
      .count = table.entries[i].count,
      
      .type = table.entries[i].type,
    };
    
    size_t j;
    
    for (j = 0; j < mbr_count; j++) {
      if (mbr[j].start + mbr[j].count <= mbr[mbr_count].start) {
        continue;
      }
      
      if (mbr[mbr_count].start + mbr[mbr_count].count <= mbr[j].start) {
        continue;
      }
      
      return 0;
    }
    
    mbr_count++;
  }
  
  if (!mbr_count) {
    return 0;
  }
  
  for (i = mbr_count; i < 4; i++) {
    mbr[i].id = -1;
  }
  
  tree->data = malloc(sizeof(mbr_t) * 4);
  memcpy(tree->data, &mbr, sizeof(mbr_t) * 4);
  
  tree->mount = mbr_mount;
  tree->free = mbr_free;
  
  tree->open = mbr_open;
  tree->list = mbr_list;
  tree->delete = mbr_delete;
  tree->close = mbr_close;
  
  log("[mbr] Mounted 'mbr' at 0x%08X, attached to device %d.\n", tree, id);
  
  for (i = 0; i < mbr_count; i++) {
    log("  => Partition %d of type 0x%02X starts at 0x%08X, has %u sectors.\n", i, mbr[i].type, (uint32_t)(mbr[i].start), (uint32_t)(mbr[i].count));
  }
  
  return 1;
}
