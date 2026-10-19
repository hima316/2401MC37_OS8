// Add to mmu.h if not already defined:
#define PDX(va)         (((uint)(va) >> PDXSHIFT) & 0x3FF)
#define PTX(va)         (((uint)(va) >> PTXSHIFT) & 0x3FF)
#define PTE_ADDR(pte)   ((uint)(pte) & ~0xFFF)
#define VADDR_OFFSET(va) ((uint)(va) & 0xFFF)