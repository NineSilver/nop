#!/usr/bin/sh

source ./prebuild.sh

rm -rf build
mkdir -p build

rm -f $(find ./nop -name "*.o") $(find ./lib -name "*.o")

echo "[nop] Building nop kernel image..."

export CFLAGS="-Iinclude -ffreestanding -O2 -fms-extensions -std=gnu89 -D__I386__ -D__NOP_KERNEL__ -D__NOP__"
export ASMFLAGS="-fcoff -D__I386__ -D__NOP_KERNEL__ -D__NOP__"

find ./nop -name "*.c" -exec i386-coff-gcc -c ${CFLAGS} {} -o {}.o \;
find ./nop -name "*.asm" -exec nasm ${ASMFLAGS} {} -o {}.o \;

find ./lib -name "*.c" -exec i386-coff-gcc -c ${CFLAGS} {} -o {}.o \;
find ./lib -name "*.asm" -exec nasm ${ASMFLAGS}{} -o {}.o \;

i386-coff-gcc -T files/i386/linker.ld -o build/nop_i386.exe -ffreestanding -O2 -nostdlib -lgcc \
  $(find ./nop -name "*.o") $(find ./lib -name "*.o")

dd if=/dev/zero of=build/test_image.img count=266248 conv=fsync status=progress

export LOOP=$(sudo losetup -f)
mkdir -p mnt

sudo losetup -P ${LOOP} build/test_image.img

sudo sfdisk ${LOOP} << EOF
  label: dos
  unit: sectors
  
  2MiB, ,
EOF

sudo mkfs.nilfs2 ${LOOP}p1

sudo mount ${LOOP}p1 mnt
sudo mkdir -p mnt/boot/grub

sudo cp build/nop_i386.exe mnt/boot/nop_i386.exe
sudo cp files/i386/grub.cfg mnt/boot/grub/grub.cfg

sudo grub-install --target=i386-pc --locales=es --modules="part_msdos nilfs2" \
  --install-modules="part_apple part_msdos part_gpt nilfs2 multiboot2" --boot-directory=mnt/boot ${LOOP}

sudo umount mnt

sudo losetup -D ${LOOP}
sudo rm -rf mnt
