void
trap(struct trapframe *tf)
{
  if(tf->trapno == T_SYSCALL){
    if(myproc()->killed)
      exit();
    myproc()->tf = tf;
    syscall();
    if(myproc()->killed)
      exit();
    return;
  }

  switch(tf->trapno){
  
  // ... [Other trap cases remain unchanged] ...

  case T_PGFLT: {
    uint va = rcr2();
    struct proc *p = myproc();

    // Verify the fault address belongs to the lazy heap region
    // Address must be below process size and above the stack/guard page
    if(p && va < p->sz && va >= 2 * PGSIZE){
      char *mem = kalloc();
      if(mem == 0){
        cprintf("lazy alloc: kalloc out of memory\n");
        p->killed = 1;
        break;
      }
      memset(mem, 0, PGSIZE);

      uint a = PGROUNDDOWN(va);
      if(mappages(p->pgdir, (char*)a, PGSIZE, V2P(mem), PTE_W | PTE_U) < 0){
        cprintf("lazy alloc: mappages failed\n");
        kfree(mem);
        p->killed = 1;
        break;
      }
      // Successfully allocated frame and updated page table
      break;
    }

    // Invalid memory access (out of bounds, stack overflow, or null pointer dereference)
    cprintf("pid %d %s: trap %d err %d on cpu %d addr 0x%x--kill proc\n",
            p->pid, p->name, tf->trapno, tf->err_code, cpuid(), va);
    p->killed = 1;
    break;
  }

  default:
    if(myproc() == 0 || (tf->cs&3) == 0){
      // In kernel, unexpected trap.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpuid(), tf->eip, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.
    cprintf("pid %d %s: trap %d err %d on cpu %d addr 0x%x--kill proc\n",
            myproc()->pid, myproc()->name, tf->trapno, tf->err_code,
            cpuid(), rcr2());
    myproc()->killed = 1;
  }

  // ... [Rest of trap function remains unchanged] ...
}