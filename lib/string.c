#include <stddef.h>
#include <stdint.h>
#include <string.h>

size_t strlen(const char *str) {
  size_t n = 0;
  
  while (str[n]) {
    n++;
  }
  
  return n;
}

void *memcpy(void *s1, const void *s2, size_t n) {
  uint8_t *s1_bytes = s1;
  const uint8_t *s2_bytes = s2;
  
  while (n--) {
    *(s1_bytes++) = *(s2_bytes++);
  }
  
  return s1;
}

void *memmove(void *s1, const void *s2, size_t n) {
  if (s2 > s1) {
    return memcpy(s1, s2, n);
  }
  
  uint8_t *s1_bytes = s1 + n;
  const uint8_t *s2_bytes = s2 + n;
  
  while (n--) {
    *(--s1_bytes) = *(--s2_bytes);
  }
  
  return s1;
}

char *strcpy(char *s1, const char *s2) {
  return memcpy(s1, s2, strlen(s2) + 1);
}

char *strncpy(char *s1, const char *s2, size_t n);
char *strcat(char *s1, const char *s2);
char *strncat(char *s1, const char *s2, size_t n);

int memcmp(const void *s1, const void *s2, size_t n) {
  const int8_t *s1_bytes = s1;
  const int8_t *s2_bytes = s2;
  
  while (n--) {
    if (*s1_bytes != *s2_bytes) {
      return *s1_bytes - *s2_bytes;
    }
    
    s1_bytes++, s2_bytes++;
  }
  
  return 0;
}

int strcmp(const char *s1, const char *s2) {
  return memcmp(s1, s2, strlen(s1));
}

int strncmp(const char *s1, const char *s2, size_t n);

const char *strchr(const char *str, char c) {
  while (*str) {
    if (*str == c) {
      return str;
    }
    
    str++;
  }
  
  return NULL;
}
