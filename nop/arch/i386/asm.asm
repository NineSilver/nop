%ifdef __I386__

[bits 32]

section .text

; String operations:

global rep_stosb
global rep_stosw
global rep_stosd

global rep_movsb
global rep_movsw
global rep_movsd

global rep_insb
global rep_insw
global rep_insd

global repe_cmpsb
global repe_cmpsw
global repe_cmpsd

global repne_cmpsb
global repne_cmpsw
global repne_cmpsd

global repe_scasb
global repe_scasw
global repe_scasd

global repne_scasb
global repne_scasw
global repne_scasd

; Port I/O:

global inb
global inw
global ind

global outb
global outw
global outd

global io_wait

; Other:

global set_cr3
global clr_cr3

global sti
global cli

global lidt

; Implementations:

rep_stosb:
  pushad
  mov al,  [esp + 32 +  4]
  mov ecx, [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep stosb
  popad
  ret

rep_stosw:
  pushad
  mov ax,  [esp + 32 +  4]
  mov ecx, [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep stosw
  popad
  ret

rep_stosd:
  pushad
  mov eax, [esp + 32 +  4]
  mov ecx, [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep stosd
  popad
  ret

rep_movsb:
  pushad
  mov ecx, [esp + 32 +  4]
  mov edi, [esp + 32 +  8]
  mov esi, [esp + 32 + 12]
  rep movsb
  popad
  ret

rep_movsw:
  pushad
  mov ecx, [esp + 32 +  4]
  mov edi, [esp + 32 +  8]
  mov esi, [esp + 32 + 12]
  rep movsw
  popad
  ret

rep_movsd:
  pushad
  mov ecx, [esp + 32 +  4]
  mov edi, [esp + 32 +  8]
  mov esi, [esp + 32 + 12]
  rep movsd
  popad
  ret

rep_insb:
  pushad
  mov ecx, [esp + 32 +  4]
  mov dx,  [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep insb
  popad
  ret

rep_insw:
  pushad
  mov ecx, [esp + 32 +  4]
  mov dx,  [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep insw
  popad
  ret

rep_insd:
  pushad
  mov ecx, [esp + 32 +  4]
  mov dx,  [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep insd
  popad
  ret

repe_cmpsb:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repe cmpsb
  mov eax, edi
  pop ecx
  pop esi
  pop edi
  ret

repe_cmpsw:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repe cmpsw
  mov eax, edi
  pop esi
  pop edi
  ret

repe_cmpsd:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repe cmpsd
  mov eax, edi
  pop esi
  pop edi
  ret

repne_cmpsb:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repne cmpsb
  mov eax, edi
  pop esi
  pop edi
  ret

repne_cmpsw:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repne cmpsw
  mov eax, edi
  pop esi
  pop edi
  ret

repne_cmpsd:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repne cmpsd
  mov eax, edi
  pop esi
  pop edi
  ret

repe_scasb:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repe scasb
  mov eax, edi
  pop ecx
  pop edi
  ret

repe_scasw:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repe scasw
  mov eax, edi
  pop ecx
  pop edi
  ret

repe_scasd:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repe scasd
  mov eax, edi
  pop ecx
  pop edi
  ret

repne_scasb:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repne scasb
  mov eax, edi
  pop ecx
  pop edi
  ret

repne_scasw:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repne scasw
  mov eax, edi
  pop ecx
  pop edi
  ret

repne_scasd:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repne scasd
  mov eax, edi
  pop ecx
  pop edi
  ret

inb:
  push edx
  mov edx, [esp + 4 + 4]
  in al, dx
  pop edx
  ret

inw:
  push edx
  mov edx, [esp + 4 + 4]
  in ax, dx
  pop edx
  ret

ind:
  push edx
  mov edx, [esp + 4 + 4]
  in eax, dx
  pop edx
  ret

outb:
  pushad
  mov eax, [esp + 32 + 4]
  mov edx, [esp + 32 + 8]
  out dx, al
  popad
  ret

outw:
  pushad
  mov eax, [esp + 32 + 4]
  mov edx, [esp + 32 + 8]
  out dx, ax
  popad
  ret

outd:
  pushad
  mov eax, [esp + 32 + 4]
  mov edx, [esp + 32 + 8]
  out dx, eax
  popad
  ret

io_wait:
  push edx
  mov dx, 0x0080
  out dx, al
  pop edx
  ret

set_cr3:
  pushad
  mov edi, [esp + 32 + 4]
  mov cr3, edi
  mov eax, cr0
  or eax, 0x80000000
  mov cr0, eax
  popad
  ret

clr_cr3:
  pushad
  mov eax, cr0
  and eax, 0x7FFFFFFF
  mov cr0, eax
  popad
  ret

sti:
  sti
  ret

cli:
  cli
  ret

lidt:
  pushad
  mov edi, [esp + 32 + 4]
  mov [idt], edi
  lidt [idt_ptr]
  popad
  ret

section .data

idt_ptr:
  dw 0x0800
idt:
  dd 0x00000000

%endif
