#!/usr/bin/sh

source ./prebuild.sh

rm -rf build
mkdir -p build

echo "[nop] Building nop kernel image..."

export CFLAGS="-Iinclude -ffreestanding -O2 -fms-extensions -std=gnu89 -D __I386__"

find ./nop -name "*.c" -exec i386-coff-gcc -c ${CFLAGS} {} -o {}.o \;
find ./lib -name "*.c" -exec i386-coff-gcc -c ${CFLAGS} {} -o {}.o \;

i386-coff-gcc -T files/i386_linker.ld -o build/nop_i386.exe -ffreestanding -O2 -nostdlib -lgcc \
  $(find ./nop -name "*.o") $(find ./lib -name "*.o")
