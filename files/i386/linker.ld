ENTRY(i386_start)

SECTIONS {
  . = 1M;
  
  i386_load_start = .;
  
  .text BLOCK(4K) : ALIGN(4K) {
    *(.header)
    *(.text)
  }
  
  .rodata BLOCK(4K) : ALIGN(4K) {
    *(.rodata)
  }
  
  .data BLOCK(4K) : ALIGN(4K) {
    *(.data)
  }
  
  i386_load_end = .;
  i386_bss_start = .;
  
  .bss BLOCK(4K) : ALIGN(4K) {
    *(COMMON)
    *(.bss)
  }
  
  i386_bss_end = .;
}
