# Lab Page tables
- [x]   print a page table
- [x]   a kernel page table per process
- [x]   simplify copyin/coyinstr
- [ ]   optional

# Reading code
- `kernel/memlayout.h`
- `kernel/vm.c`

- `kernel/kalloc.c`
- `kernel/riscv.h`: begin at 323.

- `kernel/elf.h`
- `kernel/exec.c`

# print a page table

macros:
- PTE2PA(pte)

 

# a kernel page table per process 

- CLINT: don't copy that mapping.
- `sbrt(n)`: n may not be PGSIZE aligned,
and the allocated physical pages may not be consecutive.
-`flag & ~(PTE_U)`.


