#include "types.h"
#include "stat.h"
#include "user.h"
#include "meminfo.h"

void
print_stats(const char *label, struct meminfo *mi)
{
  printf(1, "[%s]\n", label);
  printf(1, "  Total Pages : %d\n", mi->total_pages);
  printf(1, "  Free Pages  : %d\n", mi->free_pages);
  printf(1, "  User Pages  : %d (%d bytes)\n", mi->user_pages, mi->user_pages * 4096);
  printf(1, "  PT Pages    : %d\n\n", mi->pt_pages);
}

int
main(int argc, char *argv[])
{
  struct meminfo mi_initial, mi_after_sbrk, mi_child, mi_after_wait;

  printf(1, "=== Memory Statistics Test ===\n\n");

  // Step 1: Initial memory state
  if(getmeminfo(&mi_initial) < 0){
    printf(1, "getmeminfo failed!\n");
    exit();
  }
  print_stats("Initial State", &mi_initial);

  // Step 2: Grow memory using sbrk (allocate 4 pages)
  int alloc_size = 4 * 4096;
  sbrk(alloc_size);

  if(getmeminfo(&mi_after_sbrk) < 0){
    printf(1, "getmeminfo failed!\n");
    exit();
  }
  print_stats("After sbrk(+16KB)", &mi_after_sbrk);

  // Step 3: Fork child and inspect memory usage
  int free_before_fork = mi_after_sbrk.free_pages;

  int pid = fork();
  if(pid < 0){
    printf(1, "Fork failed!\n");
    exit();
  }

  if(pid == 0){
    // Child process
    if(getmeminfo(&mi_child) < 0){
      printf(1, "Child getmeminfo failed!\n");
      exit();
    }
    print_stats("Child Process State", &mi_child);
    exit();
  } else {
    // Parent process waits for child to exit
    wait();

    if(getmeminfo(&mi_after_wait) < 0){
      printf(1, "getmeminfo failed!\n");
      exit();
    }
    print_stats("After Child Exits and Reaped", &mi_after_wait);

    // Step 4: Verification for memory leaks
    if(mi_after_wait.free_pages == free_before_fork){
      printf(1, "SUCCESS: Free pages returned to pre-fork count (%d pages). No memory leak detected!\n",
             mi_after_wait.free_pages);
    } else {
      printf(1, "FAILURE: Memory leak detected! Before fork: %d, After wait: %d\n",
             free_before_fork, mi_after_wait.free_pages);
    }
  }

  exit();
}