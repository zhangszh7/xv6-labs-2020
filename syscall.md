# Lab System call

Before coding, read:
- `user/user.h`, `user/usys.pl`.
- `kernel/syscall.h`, `kernel/syscall.c`.
- `kernel/proc.h`, `kernel/proc.c`.

- [x]   system call tracing
- [x]   sysinfo
- [ ]   optional

## system call tracing

- read `trap.c`.
- add `$U/_trace\` to Makefile.
- add function signature to `user.h`.
- add a system call number to `kernel/syscall.h`.
- add an entry to `user/user.pl`. 

- add the definition of `sys_trace()` to `kernel/sysproc.c`.
- add the name to `kernel/syscall.c`.
- modify `fork()`, `syscall()`, `proc`.


functions in `kernel/sysproc.c`:
- `sys_exit`
- `sys_getpid`
- `sys_fork`
- `sys_wait`
- `sys_sbrk`
- `sys_sleep`
- `sys_kill`
- `sys_uptime`


## systeminfo

- read `user/sysinfotest.c`.
- add `$U/_sysinfotest\` to Makefile.
- add `sysinfo()`, `struct sysinfo` to `user.h`.
- add an entry to `usys.pl`.

- add `SYS_sysinfo` to `syscall.h`.
- add the prototype of `sys_sysinfo()` to `syscall.c`.

- read `kernel/sysinfo.h`.
- read and modify `kernel/kalloc.c`: add `kmem()`.
- read and modify `kernel/proc.c`: add `proc_unused()`.
- add function signatures to `kernel/defs.h`.

- add the definition of `sys_sysinfo()` to `sysproc.c`.
- read `sys_fstat()` in `sysfile.c`.
- read `filestat()` in `file.c`.

## optional


## conclusion

Compared with lab 1, lab 2 takes more attention on understanding the implementation of system calls instead of how to use them. It emphasizes on reading and understanding, so only a few lines of code need to be added in total.

- application: utilities, `user/*.c`.
- stub: get into the kernel,
put arguments and call number to registers(like a0, a7),`user/usys.pl`.
- interface: `kernel/sysproc.c`, `kernel/sysfile.c`. 
- fundament: `kernel/proc.c`, `kernel/file.c`.
- caller: `kernel/syscall.c`.































