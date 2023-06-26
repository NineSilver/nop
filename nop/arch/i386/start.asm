%ifdef __I386__

[bits 32]

extern i386_load_start
extern i386_load_end
extern i386_bss_end
extern i386_start_c

MULTIBOOT2_MAGIC  equ 0xE85250D6                  ; Multiboot2 magic number.
MULTIBOOT2_ARCH   equ 0                           ; 0 means 32-bit x86 protected mode.
MULTIBOOT2_LENGTH equ (header_end - header_start) ; Header size.

MULTIBOOT2_CHECKSUM equ (0x100000000 - (MULTIBOOT2_MAGIC + MULTIBOOT2_ARCH + MULTIBOOT2_LENGTH))

section .header
align 4

header_start:
  dd MULTIBOOT2_MAGIC
  dd MULTIBOOT2_ARCH
  dd MULTIBOOT2_LENGTH
  dd MULTIBOOT2_CHECKSUM
  
  align 8
  
  header_coff_tag:
    dw 0x0002
    dw 0x0000
    dd header_coff_tag.end - header_coff_tag
    dd header_start
    dd i386_load_start
    dd i386_load_end
    dd i386_bss_end
  .end:
  
  align 8
  
  header_entry_tag:
    dw 0x0003
    dw 0x0000
    dd header_entry_tag.end - header_entry_tag
    dd i386_start
  .end:
  
  align 8
  
  header_end_tag:
    dw 0x0000
    dw 0x0000
    dd header_end_tag.end - header_end_tag
  .end:
header_end:

section .bss
align 16

stack_bss:
  resb 16384 ; 16 KiB
.end:

section .text

global i386_start

i386_start:
  cmp eax, 0x36D76289
  jne .end
  
  mov esp, stack_bss.end
  
  push ebx
  call i386_start_c
  
.end:
  hlt
  jmp .end

%endif
