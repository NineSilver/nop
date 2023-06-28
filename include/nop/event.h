#ifndef __NOP_EVENT_H__
#define __NOP_EVENT_H__

#include <stddef.h>

#define EVENT_NAME_LENGTH 15
#define HANDLE_ALLOC_STEP 8

typedef struct handle_t handle_t;
typedef struct event_t event_t;

struct handle_t {
  char event_name[EVENT_NAME_LENGTH + 1];
  void (*func)(handle_t *handle, event_t event);
  
  void *data;
  int free;
};

struct event_t {
  char name[EVENT_NAME_LENGTH + 1];
  void *data;
};

extern handle_t *handles;
extern size_t handle_used, handle_total;

int  handle_add(handle_t handle);
int  handle_find(const char *event_name);
void handle_free(int id);

void event_trigger(event_t event);

#endif
