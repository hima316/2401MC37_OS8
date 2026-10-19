int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  struct proc *curproc = myproc();
  addr = curproc->sz;

  if(n > 0){
    // Lazy allocation: Just increase process size, don't allocate frames yet
    curproc->sz += n;
  } else if(n < 0){
    // Shrinking heap: Immediately deallocate frames
    if((addr = growproc(n)) < 0)
      return -1;
  }
  return addr;
}