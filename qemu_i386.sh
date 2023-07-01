#!/usr/bin/sh

qemu-system-i386 -m 128M -device ahci,id=ahci -device ide-hd,drive=disk0,bus=ahci.0 \
  -drive id=disk0,file=build/test_image.img,format=raw,if=none
