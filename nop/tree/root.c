#include <nop/tree/root.h>
#include <nop/device.h>
#include <nop/tree.h>
#include <nop/log.h>
#include <string.h>
#include <alloc.h>
#include <nop.h>

static void root_mount(tree_t *tree) {
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
}

static void root_free(tree_t *tree) {
  return; /* There's absolutely nothing to do here. (TODO: That's no longer the case!) */
}

static int root_open(tree_t *tree, const char *path) {
  if (*path == '$') {
    return device_find(path + 1);
  }
  
  return -1; /* We don't support root mountings as of now. */
}

static int root_list(tree_t *tree, const char *path) {
  if (*path) {
    return -1;
  }
  
  size_t i, j, count = 0;
  list_t entry;
  
  for (i = 0; i < device_total; i++) {
    count += (!devices[i].free && devices[i].is_public);
  }
  
  int list = temp_alloc("root_list", count * sizeof(list_t));
  /* TODO: Support root mountings. */
  
  for (i = 0; i < device_total; i++) {
    if (devices[i].free || !devices[i].is_public) {
      continue;
    }
    
    entry.name[0] = '$';
    strcpy(entry.name + 1, devices[i].name);
    
    entry.can_open = 1;
    entry.can_list = 0;
    
    for (j = 0; j < tree->child_count; j++) {
      if (!strcmp(tree->childs[j].name + 1, devices[i].name)) {
        entry.can_list = 1;
        break;
      }
    }
    
    device_write(list, &entry, sizeof(list_t));
  }
  
  device_seek(list, 0, SEEK_SET);
  return list;
}

static int root_delete(tree_t *tree, const char *path) {
  return 0; /* You cannot delete devices. */
}

static int root_close(tree_t *tree, int id) {
  return 0; /* We still don't support root mountings, so there's no file to close :p. */
}

int root_init(tree_t *tree, int id) {
  if (id >= 0) {
    return 0;
  }
  
  tree->mount = root_mount;
  tree->free = root_free;
  
  tree->open = root_open;
  tree->list = root_list;
  tree->delete = root_delete;
  tree->close = root_close;
  
  log("[root] Mounted 'root' at 0x%08X.\n", tree);
  return 1;
}
