#include <nop/tree/root.h>
#include <nop/device.h>
#include <nop/tree.h>
#include <nop/log.h>

static void root_free(tree_t *tree) {
  return; /* There's absolutely nothing to do here. */
}

static int root_open(tree_t *tree, const char *path) {
  if (*path == '$') {
    return device_find(path + 1);
  }
  
  return -1; /* We don't support root mountings as of now. */
}

static int root_list(tree_t *tree, const char *path) {
  if (*path) {
    return -1; /* Let the filesystem drivers list themselves. */
  }
  
  /* TODO: Not return -1. */
  return -1;
}

static int root_delete(tree_t *tree, const char *path) {
  return 0; /* You cannot delete devices. */
}

static int root_close(tree_t *tree, int id) {
  return 0; /* We still don't support root mountings, so there's no file to close :p. */
}

int root_init(tree_t *tree, int id) {
  if (id >= 0) {
    log(LOG_DEBUG, "[root] Cannot create a 'root' tree at 0x%08X, as it has a device attached (%d).\n", tree, id);
    return 0;
  }
  
  tree->free = root_free;
  
  tree->open = root_open;
  tree->list = root_list;
  tree->delete = root_delete;
  tree->close = root_close;
  
  log(LOG_INFO, "[root] Created 'root' tree at 0x%08X.\n", tree);
  return 1;
}
