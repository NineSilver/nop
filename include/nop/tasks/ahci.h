#ifndef __NOP_TASKS_AHCI_H__
#define __NOP_TASKS_AHCI_H__

#include <nop/start.h>

#define AHCI_SIG_ATA   0x00000101
#define AHCI_SIG_ATAPI 0xEB140101
#define AHCI_SIG_SEMB  0xC33C0101
#define AHCI_SIG_PM    0x96690101

typedef volatile struct hba_port_t hba_port_t;
typedef volatile struct hba_table_t hba_table_t;

typedef volatile struct hba_cmd_header_t hba_cmd_header_t;

struct hba_port_t {
  uint32_t clb;          /* 0x00: Command list base address, 1K-byte aligned. */
  uint32_t clbu;         /* 0x04: Command list base address upper 32 bits. */
  uint32_t fb;           /* 0x08: FIS base address, 256-byte aligned. */
  uint32_t fbu;          /* 0x0C: FIS base address upper 32 bits. */
  uint32_t is;           /* 0x10: Interrupt status. */
  uint32_t ie;           /* 0x14: Interrupt enable. */
  uint32_t cmd;          /* 0x18: Command and status. */
  uint32_t rsv0;         /* 0x1C: Reserved. */
  uint32_t tfd;          /* 0x20: Task file data. */
  uint32_t sig;          /* 0x24: Signature. */
  uint32_t ssts;         /* 0x28: SATA status. */
  uint32_t sctl;         /* 0x2C: SATA control. */
  uint32_t serr;         /* 0x30: SATA error. */
  uint32_t sact;         /* 0x34: SATA active. */
  uint32_t ci;           /* 0x38: command issue. */
  uint32_t sntf;         /* 0x3C: SATA notification. */
  uint32_t fbs;          /* 0x40: FIS-based switch control. */
  uint32_t reserved[11]; /* 0x44 - 0x6F: Reserved. */
  uint32_t vendor[4];    /* 0x70 - 0x7F: Vendor specific. */
} __attribute__((packed));

struct hba_table_t {
  uint32_t cap;     /* 0x00: Host capability. */
  uint32_t ghc;     /* 0x04: Global host control. */
  uint32_t is;      /* 0x08: Interrupt status. */
  uint32_t pi;      /* 0x0C: Port implemented. */
  uint32_t vs;      /* 0x10: Version. */
  uint32_t ccc_ctl; /* 0x14: Command completion coalescing control. */
  uint32_t ccc_pts; /* 0x18: Command completion coalescing ports. */
  uint32_t em_loc;  /* 0x1C: Enclosure management location. */
  uint32_t em_ctl;  /* 0x20: Enclosure management control. */
  uint32_t cap2;    /* 0x24: Host capabilities extended. */
  uint32_t bohc;    /* 0x28: BIOS/OS handoff control and status. */
  
  /* 0x2C - 0x9F: Reserved. */
  uint8_t reserved[116];
  
  /* 0xA0 - 0xFF: Vendor specific registers. */
  uint8_t vendor[96];
  
  /* Ports. */
  hba_port_t ports[32];
} __attribute__((packed));

struct hba_cmd_header_t {
  uint8_t cfl: 5; /* Command FIS length in DWORDs, 2 to 16. */
  uint8_t a: 1;   /* Is ATAPI. */
  uint8_t w: 1;   /* Write direction, 1: H2D, 0: D2H. */
  uint8_t p: 1;   /* Is prefetchable. */
  
  uint8_t r: 1;    /* Reset. */
  uint8_t b: 1;    /* BIST. */
  uint8_t c: 1;    /* Clear busy upon R_OK. */
  uint8_t zero: 1; /* Zero. */
  uint8_t pmp: 4;  /* Port multiplier port. */
  
  /* Physical region descriptor table length and transfer size. */
  uint16_t prdtl;
  uint32_t prdbc;
  
  /* Command table address. */
  uint32_t ctba;
  uint32_t ctbau;
  
  /* Reserved. */
  uint32_t reserved[4];
} __attribute__((packed));

extern const start_task_t ahci_start_task;

void ahci_task(void);

#endif
