#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_va2pa(void)
{
  int va;
  if(argint(0, &va) < 0)
    return -1;

  struct proc *curproc = myproc();
  pte_t *pte = walkpgdir(curproc->pgdir, (void*)va, 0);

  // Check if page table entry exists and is present
  if(!pte || !(*pte & PTE_P)){
    cprintf("va2pa: VA 0x%x is not present\n", va);
    return -1;
  }

  // Check if accessible from user mode (PTE_U)
  if(!(*pte & PTE_U)){
    cprintf("va2pa: VA 0x%x is not user-accessible (PTE_U not set)\n", va);
    return -1;
  }

  uint pdx = PDX(va);
  uint ptx = PTX(va);
  uint offset = VADDR_OFFSET(va);
  
  int p = (*pte & PTE_P) ? 1 : 0;
  int w = (*pte & PTE_W) ? 1 : 0;
  int u = (*pte & PTE_U) ? 1 : 0;

  // Print translation breakdown as required by Task (b)
  cprintf("va2pa: VA=0x%x -> PDIR_IDX=%d, PT_IDX=%d, OFFSET=0x%x, Flags: P=%d W=%d U=%d\n",
          va, pdx, ptx, offset, p, w, u);

  uint pa = PTE_ADDR(*pte) | offset;
  return pa;
}