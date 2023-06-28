#include <digits.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

static const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void ulong_to_str(unsigned long value, int base, char *str, size_t n) {
  if (n && !value) {
    *(str++) = '0';
    n--;
  }
  
  char buffer[40];
  int length = 0;
  
  while (n && value) {
    buffer[length++] = digits[value % base];
    value /= base;
    
    n--;
  }
  
  while (length--) {
    *(str++) = buffer[length];
  }
  
  if (n) {
    *str = '\0';
  }
}

void ulong_to_str_align(unsigned long value, int base, char *str, size_t n) {
  *(str--) = '\0';
  
  while (n && value) {
    *(str--) = digits[value % base];
    value /= base;
    
    n--;
  }
  
  while (n--) {
    *(str--) = '0';
  }
}

unsigned long str_to_ulong(int base, const char *str, size_t n) {
  unsigned long value = 0;
  
  while (n-- && *str) {
    const char *ptr = strchr(digits, toupper(*str));
    
    if (!ptr) {
      break;
    }
    
    value = value * base + (ptr - digits);
    str++;
  }
  
  return value;
}

void long_to_str(long value, int base, char *str, size_t n) {
  if (!n) {
    return;
  }
  
  if (value < 0) {
    *(str++) = '-';
    value = -value, n--;
  }
  
  ulong_to_str((unsigned long)(value), base, str, n);
}

long str_to_long(int base, const char *str, size_t n) {
  if (!n) {
    return 0;
  }
  
  int sign = (*str == '-');
  str += sign, n -= sign;
  
  long value = (long)(str_to_ulong(base, str, n));
  
  if (sign) {
    value = -value;
  }
  
  return value;
}

