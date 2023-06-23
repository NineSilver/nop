#!/usr/bin/sh

source ./prebuild.sh

rm -rf build
mkdir -p build

echo "[nop] Building nop kernel image..."

vc +files/vbcc.cfg -vv -O4 -nostdlib -Iinclude -c89 -short-push -elf -merge-constants -safe-fp \
  $(find . -maxdepth 1 -name "*.c") $(find nop -name "*.c") -o build/nop.elf
