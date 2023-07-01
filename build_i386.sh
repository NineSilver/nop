#!/usr/bin/sh

set -xe
source ./prebuild.sh

export DIRTY=0
export MKISO=0

if test "$#" -eq 1; then
  if test "$1" = "--dirty"; then
    export DIRTY=1
  fi
  
  if test "$1" = "--mkiso"; then
    export MKISO=1
  fi
fi

mkdir -p build
rm -f $(find ./nop -name "*.o") $(find ./lib -name "*.o")

echo "[nop] Building nop kernel image..."

export CFLAGS="-Iinclude -ffreestanding -O2 -fms-extensions -std=gnu89 -mno-red-zone -D__I386__ -D__NOP_KERNEL__ -D__NOP__"
export ASMFLAGS="-fcoff -D__I386__ -D__NOP_KERNEL__ -D__NOP__"

find ./nop -name "*.c" -exec i386-coff-gcc -c ${CFLAGS} {} -o {}.o \;
find ./nop -name "*.asm" -exec nasm ${ASMFLAGS} {} -o {}.o \;

find ./lib -name "*.c" -exec i386-coff-gcc -c ${CFLAGS} {} -o {}.o \;
find ./lib -name "*.asm" -exec nasm ${ASMFLAGS}{} -o {}.o \;

i386-coff-gcc -T files/i386/linker.ld -o build/nop_i386.exe -ffreestanding -O2 -nostdlib -lgcc \
  $(find ./nop -name "*.o") $(find ./lib -name "*.o")

make_test_iso() {
  mkdir -p iso/boot/grub

  cp build/nop_i386.exe iso/boot/nop_i386.exe
  cp files/i386/grub.cfg iso/boot/grub/grub.cfg

  grub-mkrescue -o build/test_iso.iso iso
  rm -rf iso
}

make_test_image() {
  if test ${DIRTY} -eq 0; then
    dd if=/dev/zero of=build/test_image.img count=266248 conv=fsync status=progress
  fi

  export LOOP=$(sudo losetup -f)
  mkdir -p mnt

  sudo losetup -P ${LOOP} build/test_image.img

  if test ${DIRTY} -eq 0; then
    sudo sfdisk ${LOOP} << EOF
      label: dos
      unit: sectors
      
      2MiB, ,
EOF
    
    sudo mkfs.nilfs2 ${LOOP}p1
  fi

  sudo mount ${LOOP}p1 mnt
  sudo mkdir -p mnt/boot/grub

  sudo cp build/nop_i386.exe mnt/boot/nop_i386.exe
  sudo cp files/i386/grub.cfg mnt/boot/grub/grub.cfg

  if test ${DIRTY} -eq 0; then
    sudo grub-install --target=i386-pc --locales=es --modules="part_msdos nilfs2" \
      --install-modules="part_apple part_msdos part_gpt nilfs2 multiboot2" \
      --boot-directory=mnt/boot ${LOOP}
  fi

  sudo umount mnt
  sudo losetup -D ${LOOP}
  
  rm -rf mnt
}

if test ${MKISO} -eq 1; then
  make_test_iso
else
  make_test_image
fi
