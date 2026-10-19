# Assignment 8: xv6 Virtual Memory & Address Translation Labs

## Overview of Questions & Features

- **Q1 — Virtual-to-Physical Address Translation (`va2pa`):** Adds a system call to walk two-level page tables, retrieve physical frame mappings and permission flags (`P`, `W`, `U`), and verify address translation across process isolation boundaries after `fork()`.

- **Q2 — Physical Memory Statistics (`getmeminfo`):** Tracks global physical memory frame allocation (`free_pages` and `total_pages`) and process-specific memory consumption.

- **Q3 — Process Page Table Inspection (`vmprint`):** Implements hierarchical printing of process page directories and user-space page tables, highlighting the stack guard page where the user-accessible flag (`PTE_U`) is cleared.

- **Q4 — Lazy Memory Allocation (`sbrk` and `T_PGFLT`):** Modifies heap allocation to defer physical memory allocation until a page fault occurs when an allocated virtual address is accessed.

---

## Build and Run Instructions

### 1. Compile and Launch xv6

Build the kernel and launch the QEMU emulator:

```bash
make qemu-nox
```

### 2. Run the Tests

Execute the following test programs from the xv6 shell:

**Q1 — Virtual-to-Physical Address Translation**
```bash
vatest
```

**Q2 — Physical Memory Statistics and Leak Verification**
```bash
memtest
```

**Q3 — Page Table Inspection and Guard Page Identification**
```bash
vmprinttest
```

**Q4 — Lazy Memory Allocation and Demand Paging**
```bash
lazytest
```

### 3. Exit QEMU

To exit the QEMU emulator, press:

`Ctrl+A`, then `X`.