#include <nop/log.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <alloc.h>

int log_is_early;

char *log_buffer;
size_t log_offset, log_size;

void (*log_write_ptr)(const char *str, size_t n);

void log_early(void (*write)(const char *str, size_t n)) {
  log_is_early = 1;
  log_write_ptr = write;
}

void log_init(const char *name) {
  log_is_early = 0;
  
  log_buffer = NULL;
  
  log_offset = 0;
  log_size = 0;
  
  /* TODO: Setup device stuff. */
}

void log_devices(const char **devices, size_t device_count) {
  /* TODO: Setup more device stuff. */
}

void log_write(const char *str, size_t n) {
  log_write_ptr(str, n);
  
  if (!log_is_early) {
    if (log_size < log_offset + n) {
      log_size = log_offset + n;
      
      log_size = 1 + (log_size + (LOG_ALLOC_SIZE - 1)) / LOG_ALLOC_SIZE;
      log_size *= LOG_ALLOC_SIZE;
      
      log_buffer = realloc(log_buffer, log_size);
    }
    
    memcpy(log_buffer + log_offset, str, n);
    log_offset += n;
  }
}

static void putchar(char chr) {
  log_write(&chr, 1);
}

static void puts_opt(const char *str, int pad_aln, int pad_len, char pad_chr) {
  int length = strlen(str) + 1;

  if (pad_aln) {
    while (pad_len >= length) {
      putchar(pad_chr);
      pad_len--;
    }
  }
  
  log_write(str, length - 1);
  
  if (!pad_aln) {
    while (pad_len >= length) {
      putchar(pad_chr);
      pad_len--;
    }
  }
}

static void putn_opt(int num, int base, int upper, int pad_aln, int pad_len, char pad_chr) {
  char buffer[100] = {0};
  int offset = 99;

  const char *digits_upper = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const char *digits_lower = "0123456789abcdefghijklmnopqrstuvwxyz";
  const char *digits = (upper ? digits_upper : digits_lower);

  unsigned int value = (unsigned int)(num);

  if (num < 0 && base == 10) {
    value = (unsigned int)(-num);
  }

  if (num == 0) {
    buffer[--offset] = '0';
  } else {
    while (value) {
      buffer[--offset] = digits[value % base];
      value /= base;
    }
  }

  if (num < 0 && base == 10) {
    buffer[--offset] = '-';
  }

  puts_opt(buffer + offset, pad_aln, pad_len, pad_chr);
}

void log(const char *format, ...) {
  va_list args;
  va_start(args, format);
  
  while (*format) {
    if (*format == '%') {
      format++;
      
      int pad_aln = 1;
      int pad_len = 0;
      char pad_chr = ' ';
      
      if (*format == '-') {
        pad_aln = 0;
        format++;
      }
      
      if (*format == '*') {
        pad_len = va_arg(args, int);
        format++;
      }
      
      if (*format == '0' || *format == '.') {
        pad_chr = '0';
        format++;
      }
      
      while (*format >= '0' && *format <= '9') {
        pad_len *= 10;
        pad_len += (*format - '0');
        format++;
      }
      
      switch (*format) {
        case '%':
          putchar('%');
          break;
          
        case 'c':
          putchar((char)(va_arg(args, int)));
          break;
          
        case 'i': case 'd': case 'u':
          putn_opt(va_arg(args, int), 10, 0, pad_aln, pad_len, pad_chr);
          break;
          
        case 'p':
          pad_len = sizeof(size_t) * 2;
          pad_chr = '0';
        case 'x':
          putn_opt(va_arg(args, int), 16, 0, pad_aln, pad_len, pad_chr);
          break;
          
        case 'P':
          pad_len = sizeof(size_t) * 2;
          pad_chr = '0';
        case 'X':
          putn_opt(va_arg(args, int), 16, 1, pad_aln, pad_len, pad_chr);
          break;
          
        case 'o':
          putn_opt(va_arg(args, int), 8, 0, pad_aln, pad_len, pad_chr);
          break;
          
        case 'b':
          putn_opt(va_arg(args, int), 2, 0, pad_aln, pad_len, pad_chr);
          break;
          
        case 's':
          puts_opt(va_arg(args, const char *), pad_aln, pad_len, pad_chr);
          break;
      }
    } else {
      putchar(*format);
    }
    
    format++;
  }
  
  va_end(args);
}
