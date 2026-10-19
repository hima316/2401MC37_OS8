// Add under // vm.c section in defs.h:
pte_t*          walkpgdir(pde_t *pgdir, const void *va, int alloc);
int             sys_va2pa(void);