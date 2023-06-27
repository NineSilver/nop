#ifndef __NOP_ARCH_I386_ASM_H__
#define __NOP_ARCH_I386_ASM_H__

#include <stdint.h>

// String operations:

extern void rep_stosb(uint8_t al, uint32_t ecx, void *edi);
extern void rep_stosw(uint16_t ax, uint32_t ecx, void *edi);
extern void rep_stosd(uint32_t eax, uint32_t ecx, void *edi);

extern void rep_movsb(uint32_t ecx, void *edi, const void *esi);
extern void rep_movsw(uint32_t ecx, void *edi, const void *esi);
extern void rep_movsd(uint32_t ecx, void *edi, const void *esi);

extern void rep_insb(uint32_t ecx, uint16_t dx, void *edi);
extern void rep_insw(uint32_t ecx, uint16_t dx, void *edi);
extern void rep_insd(uint32_t ecx, uint16_t dx, void *edi);

extern const void *repe_cmpsb(uint32_t ecx, const void *edi, const void *esi);
extern const void *repe_cmpsw(uint32_t ecx, const void *edi, const void *esi);
extern const void *repe_cmpsd(uint32_t ecx, const void *edi, const void *esi);

extern const void *repne_cmpsb(uint32_t ecx, const void *edi, const void *esi);
extern const void *repne_cmpsw(uint32_t ecx, const void *edi, const void *esi);
extern const void *repne_cmpsd(uint32_t ecx, const void *edi, const void *esi);

extern const void *repe_scasb(uint8_t al, uint32_t ecx, const void *edi);
extern const void *repe_scasw(uint16_t ax, uint32_t ecx, const void *edi);
extern const void *repe_scasd(uint32_t eax, uint32_t ecx, const void *edi);

extern const void *repne_scasb(uint8_t al, uint32_t ecx, const void *edi);
extern const void *repne_scasw(uint16_t ax, uint32_t ecx, const void *edi);
extern const void *repne_scasd(uint32_t eax, uint32_t ecx, const void *edi);

// Port I/O:

extern uint8_t  inb(uint16_t dx);
extern uint16_t inw(uint16_t dx);
extern uint32_t ind(uint16_t dx);

extern void outb(uint8_t al, uint16_t dx);
extern void outw(uint16_t ax, uint16_t dx);
extern void outd(uint32_t eax, uint16_t dx);

extern void io_wait(void);

// Other:

extern void set_cr3(const void *edi);
extern void clr_cr3(void);

extern void sti(void);
extern void cli(void);

extern void lidt(const void *edi);

#endif
