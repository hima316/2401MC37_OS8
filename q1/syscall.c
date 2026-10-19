// Add extern declaration:
extern int sys_va2pa(void);

// Add to [SYS_va2pa] mapping table:
static int (*syscalls[])(void) = {
// ... existing syscalls ...
[SYS_va2pa]   sys_va2pa,
};