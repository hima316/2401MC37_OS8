extern int sys_vmprint(void);

static int (*syscalls[])(void) = {
// ... existing system calls ...
[SYS_vmprint] sys_vmprint,
};