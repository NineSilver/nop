#ifdef __I386__

#include <nop/arch/i386/text.h>
#include <nop/device.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

uint16_t *text_ptr;
int text_width, text_height;

int text_x, text_y;
uint16_t text_color;

int print_serial = 1;

void text_init(void *ptr, int width, int height, uint16_t color) {
  text_ptr = ptr;
  
  text_width = width;
  text_height = height;
  
  text_x = 0;
  text_y = 0;
  
  text_color = color;
}

void text_putchar(char chr) {
  if (print_serial) {
    write_serial(chr);
  }

  if (text_x >= text_width || chr == '\n') {
    text_x = 0;
    text_y++;
    
    if (text_y >= text_height) {
      text_scroll();
      text_y--;
    }
    
    if (chr == '\n') {
      return;
    }
  }
  
  text_ptr[text_x++ + text_y * text_width] = (uint16_t)(chr) | text_color;
}

void text_scroll(void) {
  memmove(text_ptr, text_ptr + text_width, text_width * (text_height - 1) * sizeof(uint16_t));
  size_t i;
  
  for (i = 0; i < text_width; i++) {
    text_ptr[i + text_width * (text_height - 1)] = text_color;
  }
}

void text_write(const char *str, size_t n) {
  while (n--) {
    text_putchar(*(str++));
  }
}

static int text_device_feature(device_t *device, int feature) {
  if (feature == FEATURE_PRESENT || feature == FEATURE_WRITE || feature == FEATURE_PAGE_SIZE) {
    return 1;
  }
  
  return 0;
}

static void text_device_commit(device_t *device) {
  return; /* Why bother commiting to a textmode terminal? */
}

static size_t text_device_write(device_t *device, const void *ptr, size_t n) {
  text_write(ptr, n);
  return 0;
}

static size_t text_device_read(device_t *device, void *ptr, size_t n) {
  return 0; /* Nah-ah, just check the friggin' features, dammit! */
}

static void text_device_seek(device_t *device, ssize_t offset, int seek_mode) {
  return; /* See read(). */
}

static size_t text_device_tell(device_t *device) {
  return 0; /* See read(). */
}

static void text_device_trim(device_t *device) {
  return; /* See read(). */
}

void text_task(void) {
  if (init_serial()) {
    print_serial = 0;
  }

  device_add((device_t){
    .name = "term",
    .is_public = 1,
    
    .data = NULL,
    .free = 0,
    
    .feature = text_device_feature,
    .commit = text_device_commit,
    .write = text_device_write,
    .read = text_device_read,
    .seek = text_device_seek,
    .tell = text_device_tell,
    .trim = text_device_trim,
  }, 0);
}

#endif
