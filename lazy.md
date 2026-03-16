# Lab: lazy
- [x] eliminate allocation from sbrk
- [ ] lazy allocation
- [ ] lazytests and usertests


## eliminate allocation from sbrk

- pay attention to anything related to the process.
- More specifically, anything relates to the `myproc()->sz`.

- `freeproc()`
- `proc_freepagetable()`
- `uvmfree()`
- `uvmunmap()`: va, npages, `do_free`

- `fork()`
- `uvmcopy()`
 




