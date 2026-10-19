int
sys_vmprint(void)
{
  struct proc *curproc = myproc();
  vmprint(curproc->pgdir);
  return 0;
}