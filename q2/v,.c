int
count_pt_pages(pde_t *pgdir)
{
  int count = 1; // 1 page for the Page Directory itself
  int i;

  for(i = 0; i < NPDENTRIES; i++){
    if(pgdir[i] & PTE_P){
      count++; // 1 page for each present Page Table
    }
  }
  return count;
}