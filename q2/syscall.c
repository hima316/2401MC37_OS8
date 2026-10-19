extern int sys_getmeminfo(void);

static int (*syscalls[])(void) = {
// ... existing system calls ...
[SYS_getmeminfo] sys_getmeminfo,
};