#ifndef _MEMINFO_H_
#define _MEMINFO_H_

struct meminfo {
  int total_pages;      // Total physical pages managed by kalloc
  int free_pages;       // Currently available free physical pages
  int user_pages;       // Number of user-space pages (sz / PGSIZE)
  int pt_pages;         // Number of pages used by the process's page tables
};

#endif