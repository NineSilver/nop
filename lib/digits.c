#include <digits.h>
#include <stddef.h>

void ulong_to_str(unsigned long value, int base, char *str, size_t n) {
  /* TODO */
  value; base; str; n;
}

unsigned long str_to_ulong(int base, const char *str, size_t n) {
  unsigned long value = 0;
  
  while (n-- && *str) {
    /* TODO: Add support for hexadecimal, detect the end of the number. */
    
    value = value * base + (*str - '0');
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
