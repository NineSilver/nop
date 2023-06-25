#include <nop/fs.h>

const fs_type_t fs_types[] = {
  (fs_type_t){
    .name = "MBR",
    
    .init = mbr_init,
    .free = mbr_free,
  },
};

const int fs_type_count = sizeof(fs_types) / sizeof(fs_type_t);

fs_t fs_root;

void fs_init(void) {
  /* TODO: Setup fs_root. */
}

int fs_open(const char *path) {
  return fs_root.open(&fs_root, path);
}

int fs_list(const char *path) {
  return fs_root.list(&fs_root, path);
}

void fs_delete(const char *path) {
  fs_root.delete(&fs_root, path);
}

void fs_close(int id) {
  fs_root.close(&fs_root, id);
}
