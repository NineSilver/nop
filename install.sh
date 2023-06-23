#!/usr/bin/sh

DEVICE=$1

# TODO: Use -p1 instead of -1 on loop devices!
PARTITION=${DEVICE}1

if test ${EUID} -ne 0; then
  echo "[nop] ERROR: This script must be run as root!"
  exit 1
fi

if test -z ${DEVICE}; then
  echo "[nop] ERROR: Must specify a device or loop device to install to:"
  echo "  $ sh install.sh /dev/sdc"
  exit 1
fi

echo "[nop] 1. Partitioning device..."

sfdisk ${DEVICE} << EOF
  label: dos
  unit: sectors
  
  8MiB, ,
EOF

echo "[nop] 2. Creating ext2 filesystem..."

mkfs.ext2 ${PARTITION}

echo "[nop] Done!"
