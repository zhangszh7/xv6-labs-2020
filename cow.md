# Lab: Copy-on-Write Fork  

## downside
- The count array is too big, which almost costs 2MB and most are unused.
- The Structure of the trap handler is the same as that of the copyout handler,
so it's wise to define a new function for clear and sharing.

