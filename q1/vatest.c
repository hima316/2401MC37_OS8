#include "types.h"
#include "stat.h"
#include "user.h"

int global_var = 42;

int
main(int argc, char *argv[])
{
  int local_var = 100;
  char *heap_var = (char*) malloc(100);
  heap_var[0] = 'A';

  printf(1, "--- Testing Virtual to Physical Translation ---\n");

  int pa_global = va2pa((int)&global_var);
  printf(1, "Global Var: VA = 0x%x, PA = 0x%x\n", (uint)&global_var, pa_global);

  int pa_local = va2pa((int)&local_var);
  printf(1, "Local Var:  VA = 0x%x, PA = 0x%x\n", (uint)&local_var, pa_local);

  int pa_heap = va2pa((int)heap_var);
  printf(1, "Heap Var:   VA = 0x%x, PA = 0x%x\n", (uint)heap_var, pa_heap);

  printf(1, "\n--- Testing Unmapped / High Address ---\n");
  int invalid_va = 0x7FFFFFFF;
  int pa_invalid = va2pa(invalid_va);
  printf(1, "Invalid VA 0x%x -> PA = %d (Expected: -1)\n", invalid_va, pa_invalid);

  printf(1, "\n--- Testing Fork Behavior (COW / Independent Pages) ---\n");
  int pid = fork();
  if(pid < 0){
    printf(1, "Fork failed!\n");
    exit();
  }

  if(pid == 0){
    // Child process
    int child_pa = va2pa((int)&global_var);
    printf(1, "[CHILD]  Global Var: VA = 0x%x, PA = 0x%x\n", (uint)&global_var, child_pa);
    free(heap_var);
    exit();
  } else {
    // Parent process
    wait();
    int parent_pa = va2pa((int)&global_var);
    printf(1, "[PARENT] Global Var: VA = 0x%x, PA = 0x%x\n", (uint)&global_var, parent_pa);
  }

  free(heap_var);
  exit();
}