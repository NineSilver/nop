# nopOS: A portable monolithic OS, from the ground up.

nopOS is meant to be a long-term hobby project, whose main goal is to
provide a fully usable and self-hosting enviroment on many different
architectures and devices, ranging from (hopefully) simple 8-bit
microcontrollers to fast and modern x86_64 CPUs, to really obscure
server-specific ISAs.

nopOS is divided into its main three parts, named after their matching
folders:

- nop: nopOS's kernel, written in GNU89-compliant C code.
- lib: An alternative (yet largely overlapping) standard library for both nop and nopOS's userspace.
- host: A complete suite of programs, ports and configuration files for nopOS's userspace.

# nop, nopOS's kernel:

TODO

## Progress:

- nop
  - [x] Device system (nop/device).
  - [x] Tree system (nop/tree).
    - [ ] root
    - [ ] pci
    - [ ] net
    - [ ] mbr
    - [ ] gpt
    - [ ] nilfs2
    - [ ] fat12
    - [ ] fat16
    - [ ] fat32
  - [ ] Event system (nop/event).
  - [ ] Running unit system (nop/unit).
    - [ ] Kernel call handling (with BSON).
  - [ ] Make codebase SMP-friendly (sounds like a huge task to me, I have no idea how to...).
- i386
  - [x] Entry point.
  - PCI (nop/arch/i386/pci).
    - [ ] Detect PCI presence instead of assuming it's there.
    - [ ] Write support.
    - [x] Read support.
  - [x] VGA text mode (nop/arch/i386/text).
  - [x] Serial ports (nop/arch/i386/serial).
  - [ ] AHCI (nop/arch/i386/ahci).
- x86_64
  - [ ] Entry point.
- armv7/v8
  - [ ] Entry point.
- aarch64
  - [ ] Entry point.
- rv32* (RISC-V)
  - [ ] Entry point.
- rv64* (RISC-V)
  - [ ] Entry point.
- SH4A
  - [ ] Entry point.

# lib:

TODO

## Progress:

TODO

# host:

TODO

## Progress:

TODO
