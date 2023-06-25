#ifndef __DIGITS_H__
#define __DIGITS_H__

#include <stddef.h>

void          ulong_to_str(unsigned long value, int base, char *str, size_t n);
unsigned long str_to_ulong(int base, const char *str, size_t n);

void long_to_str(long value, int base, char *str, size_t n);
long str_to_long(int base, const char *str, size_t n);

#endif
