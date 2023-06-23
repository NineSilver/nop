#include <endian.h>
#include <stdint.h>

int16_t int16le_to_host(int16le_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (int16_t)(
    ((uint16_t)(bytes[0]) << 0) |
    ((uint16_t)(bytes[1]) << 8)
  );
}

int16_t int16be_to_host(int16be_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (int16_t)(
    ((uint16_t)(bytes[0]) << 8) |
    ((uint16_t)(bytes[1]) << 0)
  );
}

int32_t int32le_to_host(int32le_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (int32_t)(
    ((uint32_t)(bytes[0]) << 0) |
    ((uint32_t)(bytes[1]) << 8) |
    ((uint32_t)(bytes[2]) << 16) |
    ((uint32_t)(bytes[3]) << 24)
  );
}

int32_t int32be_to_host(int32be_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (int32_t)(
    ((uint32_t)(bytes[0]) << 24) |
    ((uint32_t)(bytes[1]) << 16) |
    ((uint32_t)(bytes[2]) << 8) |
    ((uint32_t)(bytes[3]) << 0)
  );
}

int64_t int64le_to_host(int64le_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (int64_t)(
    ((uint64_t)(bytes[0]) << 0) |
    ((uint64_t)(bytes[1]) << 8) |
    ((uint64_t)(bytes[2]) << 16) |
    ((uint64_t)(bytes[3]) << 24) |
    ((uint64_t)(bytes[4]) << 32) |
    ((uint64_t)(bytes[5]) << 40) |
    ((uint64_t)(bytes[6]) << 48) |
    ((uint64_t)(bytes[7]) << 56)
  );
}

int64_t int64be_to_host(int64be_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (int64_t)(
    ((uint64_t)(bytes[0]) << 56) |
    ((uint64_t)(bytes[1]) << 48) |
    ((uint64_t)(bytes[2]) << 40) |
    ((uint64_t)(bytes[3]) << 32) |
    ((uint64_t)(bytes[4]) << 24) |
    ((uint64_t)(bytes[5]) << 16) |
    ((uint64_t)(bytes[6]) << 8) |
    ((uint64_t)(bytes[7]) << 0)
  );
}

uint16_t uint16le_to_host(uint16le_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (
    ((uint16_t)(bytes[0]) << 0) |
    ((uint16_t)(bytes[1]) << 8)
  );
}

uint16_t uint16be_to_host(uint16be_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (
    ((uint16_t)(bytes[0]) << 8) |
    ((uint16_t)(bytes[1]) << 0)
  );
}

uint32_t uint32le_to_host(uint32le_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (
    ((uint32_t)(bytes[0]) << 0) |
    ((uint32_t)(bytes[1]) << 8) |
    ((uint32_t)(bytes[2]) << 16) |
    ((uint32_t)(bytes[3]) << 24)
  );
}

uint32_t uint32be_to_host(uint32be_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (
    ((uint32_t)(bytes[0]) << 24) |
    ((uint32_t)(bytes[1]) << 16) |
    ((uint32_t)(bytes[2]) << 8) |
    ((uint32_t)(bytes[3]) << 0)
  );
}

uint64_t uint64le_to_host(uint64le_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (
    ((uint64_t)(bytes[0]) << 0) |
    ((uint64_t)(bytes[1]) << 8) |
    ((uint64_t)(bytes[2]) << 16) |
    ((uint64_t)(bytes[3]) << 24) |
    ((uint64_t)(bytes[4]) << 32) |
    ((uint64_t)(bytes[5]) << 40) |
    ((uint64_t)(bytes[6]) << 48) |
    ((uint64_t)(bytes[7]) << 56)
  );
}

uint64_t uint64be_to_host(uint64be_t n) {
  uint8_t *bytes = (uint8_t *)(&n);
  
  return (
    ((uint64_t)(bytes[0]) << 56) |
    ((uint64_t)(bytes[1]) << 48) |
    ((uint64_t)(bytes[2]) << 40) |
    ((uint64_t)(bytes[3]) << 32) |
    ((uint64_t)(bytes[4]) << 24) |
    ((uint64_t)(bytes[5]) << 16) |
    ((uint64_t)(bytes[6]) << 8) |
    ((uint64_t)(bytes[7]) << 0)
  );
}
