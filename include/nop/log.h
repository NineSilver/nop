#ifndef __NOP_LOG_H__
#define __NOP_LOG_H__

#include <stddef.h>

#define LOG_ALLOC_SIZE 4096

#define LOG_FATAL    0
#define LOG_CRITICAL 1
#define LOG_ERROR    2
#define LOG_WARNING  3
#define LOG_INFO     4
#define LOG_DEBUG    5

extern int log_is_early;

extern char *log_buffer;
extern size_t log_offset, log_size;

extern void (*log_write_ptr)(const char *str, size_t n);

void log_early(void (*write)(const char *str, size_t n));
void log_init(const char *name);
void log_devices(const char **devices, size_t device_count);

void log_write(const char *str, size_t n);

void log(int level, const char *format, ...);

#endif
