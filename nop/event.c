#include <nop/event.h>
#include <stdint.h>
#include <string.h>
#include <alloc.h>

handle_t *handles = NULL;
size_t handle_used = 0, handle_total = 0;

int handle_add(handle_t handle) {
  if (handle_used >= handle_total) {
    handles = realloc(handles, (handle_total + HANDLE_ALLOC_STEP) * sizeof(handle_t));
    handle_total += HANDLE_ALLOC_STEP;
  }
  
  size_t i;
  
  for (i = 0; i < handle_total; i++) {
    if (handles[i].free) {
      handles[i] = handle;
      handles[i].free = 0;
      
      handle_used++;
      return i;
    }
  }
  
  return -1;
}

int handle_find(const char *event_name) {
  size_t i;
  
  for (i = 0; i < handle_total; i++) {
    if (!handles[i].free && !strncmp(handles[i].event_name, event_name, EVENT_NAME_LENGTH)) {
      return i;
    }
  }
  
  return -1;
}

void handle_free(int id) {
  if (id < 0 || handles[id].free) {
    return;
  }
  
  handles[id].free = 1;
  handle_used--;
}

void event_trigger(event_t event) {
  size_t i;
  
  for (i = 0; i < handle_total; i++) {
    if (!handles[i].free && !strncmp(handles[i].event_name, event.name, EVENT_NAME_LENGTH)) {
      handles[i].func(handles + i, event);
    }
  }
}
