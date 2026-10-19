void
vmprint(pde_t *pgdir)
{
  cprintf("page table %p\n", pgdir);

  int kernel_pte_count = 0;

  // Walk through the Page Directory
  for(int i = 0; i < NPDENTRIES; i++){
    pde_t pde = pgdir[i];
    if(pde & PTE_P){
      uint pd_pa = PTE_ADDR(pde);
      
      // Calculate virtual address start corresponding to this PDE
      uint pd_va = i << PDXSHIFT;

      // Filter: Process user space entries (below KERNBASE)
      if(pd_va < KERNBASE){
        cprintf("..[%d] pde: pa 0x%x, flags 0x%x\n", i, pd_pa, pde & 0xFFF);
        
        pte_t *pgtab = (pte_t*)P2V(pd_pa);
        for(int j = 0; j < NPTENTRIES; j++){
          pte_t pte = pgtab[j];
          if(pte & PTE_P){
            uint va = (i << PDXSHIFT) | (j << PTXSHIFT);
            uint pa = PTE_ADDR(pte);
            uint flags = pte & 0xFFF;

            int p = (flags & PTE_P) ? 1 : 0;
            int w = (flags & PTE_W) ? 1 : 0;
            int u = (flags & PTE_U) ? 1 : 0;

            cprintf("  ..[%d] pte: va 0x%x -> pa 0x%x, flags [P=%d W=%d U=%d] (0x%x)\n",
                    j, va, pa, p, w, u, flags);
          }
        }
      } else {
        // Count kernel mappings above KERNBASE
        pte_t *pgtab = (pte_t*)P2V(pd_pa);
        for(int j = 0; j < NPTENTRIES; j++){
          if(pgtab[j] & PTE_P){
            kernel_pte_count++;
          }
        }
      }
    }
  }

  cprintf("Summary: %d valid kernel page table entries above KERNBASE.\n", kernel_pte_count);
}