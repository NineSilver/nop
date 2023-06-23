- ext2 OR btrfs filesystem (no boot partition)
- Arquitecture-independent (supports x86, x86_64 and more?)

NOTE: nop expects vasm, vlink and vbcc to be installed (http://www.compilers.de/).

## IPC:

Why not just use an object format like BSON? (https://bsonspec.org/)

## Filesystems:

We should look for alternative filesystems, otherwise, just use btrfs and 5 years of my
life to implement it properly. My main concern is how slow btrfs could be on simpler
hardware. Maybe LEAN? (https://wiki.osdev.org/LEAN)

## nCOFF and nCOFF64:

nCOFF is just plain COFF but with many more supported architectures, even 16-bit (though
just padded in 32-bit values). Additionally, several signed values are turned unsigned,
like time stamps, file offsets and sizes.

nCOFF64 extends several structures:
- Section headers (from 40 to 64 bytes).
- 
