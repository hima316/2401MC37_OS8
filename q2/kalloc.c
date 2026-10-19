#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  int free_pages;     // Track currently free pages
  int total_pages;    // Track total managed pages
} kmem;

void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);

  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  kmem.free_pages++;

  if(kmem.use_lock)
    release(&kmem.lock);
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE){
    kfree(p);
    // Count total pages added during initialization
    if(!kmem.use_lock)
      kmem.total_pages++;
  }
}

char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);
  
  r = kmem.freelist;
  if(r){
    kmem.freelist = r->next;
    kmem.free_pages--;
  }

  if(kmem.use_lock)
    release(&kmem.lock);

  return (char*)r;
}

int
get_free_pages(void)
{
  int count;
  acquire(&kmem.lock);
  count = kmem.free_pages;
  release(&kmem.lock);
  return count;
}

int
get_total_pages(void)
{
  return kmem.total_pages;
}