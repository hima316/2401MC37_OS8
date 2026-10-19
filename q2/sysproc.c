#include "meminfo.h"

int
sys_getmeminfo(void)
{
  struct meminfo *mi;

  if(argptr(0, (void**)&mi, sizeof(*mi)) < 0)
    return -1;

  struct proc *curproc = myproc();

  mi->total_pages = get_total_pages();
  mi->free_pages = get_free_pages();
  mi->user_pages = curproc->sz / PGSIZE;
  mi->pt_pages = count_pt_pages(curproc->pgdir);

  return 0;
}