#ifndef __NOP_TREE_H__
#define __NOP_TREE_H__

#include <nop/device.h>

/* root, pci, mbr, gpt, ext2, tcp, http, https, etc. */

#define TREE_NAME_LENGTH 15

typedef struct tree_type_t tree_type_t;
typedef struct tree_t tree_t;

struct tree_type_t {
  int (*init)(tree_t *tree, int id);
};

struct tree_t {
  char name[TREE_NAME_LENGTH + 1];
  void *data;
  
  tree_t *childs;
  size_t child_count;
  
  void (*mount)(tree_t *tree);
  void (*free)(tree_t *tree);
  
  int (*open)(tree_t *tree, const char *path);
  int (*list)(tree_t *tree, const char *path); /* Instead of opening the specified path, generate a temp. file with listings. */
  int (*delete)(tree_t *tree, const char *path);
  int (*close)(tree_t *tree, int id);
};

extern const tree_type_t tree_types[];
extern const int tree_type_count;

extern tree_t root;

int  tree_init(tree_t *tree, int id);
void tree_mount(tree_t *tree);
void tree_free(tree_t *tree);

int tree_is_tree(tree_t *tree, const char *path);

int tree_open(tree_t *tree, const char *path);
int tree_list(tree_t *tree, const char *path);
int tree_delete(tree_t *tree, const char *path);
int tree_close(tree_t *tree, int id);

#endif
