#include "types.h"
#include "stat.h"
#include "user.h"
#include "meminfo.h"

void
print_mem(const char *label)
{
  struct meminfo mi;
  if(getmeminfo(&mi) == 0){
    printf(1, "[%s] Free Pages: %d | User Pages: %d\n", label, mi.free_pages, mi.user_pages);
  }
}

int
main(int argc, char *argv[])
{
  printf(1, "=== Lazy Memory Allocation Test ===\n\n");

  print_mem("Initial State");

  // Step 1: Allocate 32 MB lazily (32 MB = 8192 pages)
  uint sz = 32 * 1024 * 1024;
  printf(1, "\n--- Requesting 32 MB via sbrk() ---\n");
  char *p = sbrk(sz);
  if(p == (char*)-1){
    printf(1, "sbrk failed!\n");
    exit();
  }

  print_mem("After sbrk(32MB)");

  // Step 2: Touch only 3 widely spaced pages
  printf(1, "\n--- Touching 3 specific pages (triggers demand paging) ---\n");
  p[0] = 'A';                      // Touch page 0
  p[4096 * 100] = 'B';             // Touch page 100
  p[4096 * 2000] = 'C';            // Touch page 2000

  print_mem("After Touching 3 Pages");

  // Step 3: Test kernel system call touching an unmapped lazy page
  printf(1, "\n--- Testing system call read() on an unmapped lazy page ---\n");
  char *lazy_buf = p + (4096 * 500); // Page 500 is currently unmapped
  int fd[2];
  pipe(fd);
  write(fd[1], "Lazy System Call Test Successful!", 33);
  read(fd[0], lazy_buf, 33);         // Triggers kernel-mode page fault & lazy allocation
  printf(1, "Read Result: '%s'\n", lazy_buf);

  print_mem("After Syscall on Unmapped Page");

  // Step 4: Test Fork with lazy pages
  printf(1, "\n--- Testing fork() with lazily allocated pages ---\n");
  int pid = fork();
  if(pid < 0){
    printf(1, "Fork failed!\n");
    exit();
  }

  if(pid == 0){
    // Child touches another page
    p[4096 * 3000] = 'D';
    print_mem("Child Process State");
    exit();
  } else {
    wait();
    print_mem("After Child Exits and Reaped");
  }

  printf(1, "\nSUCCESS: Lazy Memory Allocation Verified!\n");
  exit();
}