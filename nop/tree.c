#include <nop/tree/root.h>
#include <nop/tree.h>
#include <string.h>
#include <alloc.h>

const tree_type_t tree_types[] = {
  (tree_type_t){root_init},
};

const int tree_type_count = sizeof(tree_types) / sizeof(tree_type_t);

tree_t root;

int tree_init(tree_t *tree, int id) {
  size_t i;
  
  for (i = 0; i < tree_type_count; i++) {
    if (tree_types[i].init(tree, id)) {
      return 1;
    }
  }
  
  return 0;
}

void tree_free(tree_t *tree) {
  size_t i;
  
  for (i = 0; i < tree->child_count; i++) {
    tree_free(tree->childs + i);
  }
  
  free(tree->childs);
  tree->free(tree);
}

int tree_is_tree(tree_t *tree, const char *path) {
  while (*path == '/') {
    path++;
  }
  
  if (!(*path)) {
    return 1;
  }
  
  char name[TREE_NAME_LENGTH + 1];
  
  const char *ptr = strchrnul(path, '/');
  size_t length = ptr - path;
  
  memcpy(name, path, length);
  name[length] = '\0';
  
  size_t i;
  
  for (i = 0; i < tree->child_count; i++) {
    if (!strcmp(tree->childs[i].name, name)) {
      return tree_is_tree(tree->childs + i, ptr);
    }
  }
  
  return 0;
}

int tree_open(tree_t *tree, const char *path) {
  int id = tree->open(tree, path);
  size_t i;
  
  if (id >= 0) {
    return id;
  }
  
  for (i = 0; i < tree->child_count; i++) {
    id = tree_open(tree->childs + i, path);
    
    if (id >= 0) {
      return id;
    }
  }
  
  return -1;
}

int tree_list(tree_t *tree, const char *path) {
  while (*path == '/') {
    path++;
  }
  
  int id = tree->list(tree, path);
  size_t i;
  
  if (id >= 0) {
    return id;
  }
  
  for (i = 0; i < tree->child_count; i++) {
    id = tree_list(tree->childs + i, path);
    
    if (id >= 0) {
      return id;
    }
  }
  
  return -1;
}

int tree_delete(tree_t *tree, const char *path) {
  // tree_root.delete(&tree_root, path);
}

int tree_close(tree_t *tree, int id) {
  // tree_root.close(&tree_root, id);
}
