#ifndef __NOP_FS_H__
#define __NOP_FS_H__

/* mbr, gpt, ext2, tcp, http, https, etc. */

#define FS_NAME_LENGTH 15

typedef struct fs_type_t fs_type_t;
typedef struct fs_t fs_t;

struct fs_type_t {
  char name[FS_NAME_LENGTH + 1];
  
  int  (*init)(fs_t *fs, device_t *device);
  void (*free)(fs_t *fs);
};

struct fs_t {
  const fs_type_t *type;
  void *data;
  
  int  (*open)(fs_t *fs, const char *path);
  int  (*list)(fs_t *fs, const char *path); /* Instead of opening the specified path, generate a temp. file with listings. */
  void (*delete)(fs_t *fs, const char *path);
  void (*close)(fs_t *fs, int id);
};

extern const fs_type_t fs_types[];
extern const int fs_type_count;

extern fs_t fs_root;

void fs_init(void);

int  fs_open(const char *path);
int  fs_list(const char *path);
void fs_delete(const char *path);
void fs_close(int id);

#endif
