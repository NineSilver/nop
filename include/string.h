#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

/* Measuring functions: */

size_t strlen(const char *str);

/* Copying functions: */

void *memcpy(void *s1, const void *s2, size_t n);
void *memmove(void *s1, const void *s2, size_t n);

char *strcpy(char *s1, const char *s2);
char *strncpy(char *s1, const char *s2, size_t n);
char *strcat(char *s1, const char *s2);
char *strncat(char *s1, const char *s2, size_t n);

/* Comparison functions: */

int memcmp(const void *s1, const void *s2, size_t n);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

/* Finding functions: */

const char *strchr(const char *str, char c);
const char *strchrnul(const char *str, char c);

/* Memory filling functions: */

void *memset(void *ptr, char c, size_t n);

#endif
