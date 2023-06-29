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
  lgdt [gdt_ptr]
  
  push ebx
  call i386_start_c
  
.end:
  hlt
  jmp .end

; Did not remove the comments for comedy's sake, as this is the
; same GDT every single OSdev-related project I have done in my
; entire life. Please, do not mind the lack of spaces after
; parentheses.

; Wow, a GDT table!
gdt:
; Even better, a GDT table pointer(in the null entry)!
gdt_ptr:
  dw (gdt_end - gdt)
  dd gdt
  dw 0x0000
gdt_code_32:
  dw 0xFFFF     ; Limit(bits 0-15)
  dw 0x0000     ; Base(bits 0-15)
  db 0x00       ; Base(bits 16-23)
  db 0b10011010 ; Access byte
  db 0b11001111 ; Flags & limit(bits 16-19)
  db 0x00       ; Base(bits 24-31)
gdt_data_32:
  dw 0xFFFF     ; Limit(bits 0-15)
  dw 0x0000     ; Base(bits 0-15)
  db 0x00       ; Base(bits 16-23)
  db 0b10010010 ; Access byte
  db 0b11001111 ; Flags & limit(bits 16-19)
  db 0x00       ; Base(bits 24-31)
gdt_code_16:
  dw 0xFFFF     ; Limit(bits 0-15)
  dw 0x0000     ; Base(bits 0-15)
  db 0x00       ; Base(bits 16-23)
  db 0b10011010 ; Access byte
  db 0b00001111 ; Flags & limit(bits 16-19)
  db 0x00       ; Base(bits 24-31)
gdt_data_16:
  dw 0xFFFF     ; Limit(bits 0-15)
  dw 0x0000     ; Base(bits 0-15)
  db 0x00       ; Base(bits 16-23)
  db 0b10010010 ; Access byte
  db 0b00001111 ; Flags & limit(bits 16-19)
  db 0x00       ; Base(bits 24-31)
gdt_end:

%endif
