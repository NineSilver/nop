#include <nop/arch/i386/textmode.h>
#include <nop/device.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

uint16_t *textmode_ptr;
int textmode_width, textmode_height;

int textmode_x, textmode_y;
uint16_t textmode_color;

void textmode_init(void *ptr, int width, int height, uint16_t color) {
  textmode_ptr = ptr;
  
  textmode_width = width;
  textmode_height = height;
  
  textmode_x = 0;
  textmode_y = 0;
  
  textmode_color = color;
}

void textmode_putchar(char chr) {
  if (textmode_x >= textmode_width || chr == '\n') {
    textmode_x = 0;
    textmode_y++;
    
    if (textmode_y >= textmode_height) {
      textmode_scroll();
      textmode_y--;
    }
    
    if (chr == '\n') {
      return;
    }
  }
  
  textmode_ptr[textmode_x++ + textmode_y * textmode_width] = (uint16_t)(chr) | textmode_color;
}

void textmode_scroll(void) {
  memmove(textmode_ptr, textmode_ptr + textmode_width, textmode_width * (textmode_height - 1) * sizeof(uint16_t));
  size_t i;
  
  for (i = 0; i < textmode_width; i++) {
    textmode_ptr[i + textmode_width * (textmode_height - 1)] = textmode_color;
  }
}

void textmode_write(const char *str, size_t n) {
  while (n--) {
    textmode_putchar(*(str++));
  }
}

static int textmode_device_feature(device_t *device, int feature) {
  if (feature == FEATURE_PRESENT || feature == FEATURE_WRITE) {
    return 1;
  }
  
  return 0;
}

static void textmode_device_commit(device_t *device) {
  return; /* Why bother commiting to a textmode terminal? */
}

static size_t textmode_device_write(device_t *device, const void *ptr, size_t n) {
  textmode_write(ptr, n);
}

static size_t textmode_device_read(device_t *device, void *ptr, size_t n) {
  return 0; /* Nah-ah, just check the friggin' features, dammit! */
}

static void textmode_device_seek(device_t *device, ssize_t offset, int seek_mode) {
  return; /* See read(). */
}

static size_t textmode_device_tell(device_t *device) {
  return 0; /* See read(). */
}

static void textmode_device_trim(device_t *device) {
  return; /* See read(). */
}

void textmode_task(void) {
  device_t device = (device_t){
    .name = "term",
    
    .data = NULL,
    .free = 0,
    
    .feature = textmode_device_feature,
    .commit = textmode_device_commit,
    .write = textmode_device_write,
    .read = textmode_device_read,
    .seek = textmode_device_seek,
    .tell = textmode_device_tell,
    .trim = textmode_device_trim,
  };
  
  device_add(device, 0);
}
