#ifndef __ENDIAN_H__
#define __ENDIAN_H__

#include <stdint.h>

typedef int16_t int16le_t;
typedef int16_t int16be_t;

typedef int32_t int32le_t;
typedef int32_t int32be_t;

typedef int64_t int64le_t;
typedef int64_t int64be_t;

typedef uint16_t uint16le_t;
typedef uint16_t uint16be_t;

typedef uint32_t uint32le_t;
typedef uint32_t uint32be_t;

typedef uint64_t uint64le_t;
typedef uint64_t uint64be_t;

int16_t int16le_to_host(int16le_t n);
int16_t int16be_to_host(int16be_t n);

int32_t int32le_to_host(int32le_t n);
int32_t int32be_to_host(int32be_t n);

int64_t int64le_to_host(int64le_t n);
int64_t int64be_to_host(int64be_t n);

uint16_t uint16le_to_host(uint16le_t n);
uint16_t uint16be_to_host(uint16be_t n);

uint32_t uint32le_to_host(uint32le_t n);
uint32_t uint32be_to_host(uint32be_t n);

uint64_t uint64le_to_host(uint64le_t n);
uint64_t uint64be_to_host(uint64be_t n);

#define host_to_int16le(n) int16le_to_host(n)
#define host_to_int16be(n) int16be_to_host(n)

#define host_to_int32le(n) int32le_to_host(n)
#define host_to_int32be(n) int32be_to_host(n)

#define host_to_int64le(n) int64le_to_host(n)
#define host_to_int64be(n) int64be_to_host(n)

#define host_to_uint16le(n) uint16le_to_host(n)
#define host_to_uint16be(n) uint16be_to_host(n)

#define host_to_uint32le(n) uint32le_to_host(n)
#define host_to_uint32be(n) uint32be_to_host(n)

#define host_to_uint64le(n) uint64le_to_host(n)
#define host_to_uint64be(n) uint64be_to_host(n)

#endif
