// Add under // kalloc.c
int             get_free_pages(void);
int             get_total_pages(void);

// Add under // vm.c
int             count_pt_pages(pde_t *pgdir);

// System call declaration
int             sys_getmeminfo(void);