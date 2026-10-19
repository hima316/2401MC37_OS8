#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "=== Page Table Dump Test ===\n\n");

  printf(1, "--- 1. Initial Page Table Dump ---\n");
  vmprint();

  printf(1, "\n--- 2. Page Table Dump After Growing Heap by 8 Pages (sbrk(+32KB)) ---\n");
  char *p = sbrk(8 * 4096);
  if(p == (char*)-1){
    printf(1, "sbrk failed!\n");
    exit();
  }
  vmprint();

  printf(1, "\n--- 3. Page Table Dump After Shrinking Heap by 1 Page (sbrk(-4KB)) ---\n");
  sbrk(-4096);
  vmprint();

  printf(1, "\n--- Verification Note ---\n");
  printf(1, "Notice the entry with VA right below the user stack where U=0 (PTE_U cleared).\n");
  printf(1, "That entry is the Stack Guard Page created by exec().\n");

  exit();
}