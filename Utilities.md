# Lab Utilities

[x]  Boot xv6
[x]  sleep

## Boot xv6

- reading the book 'progit', make a deep learning of Git.

## sleep
hints:
- look some programs in user/, like `echo.c, grep.c, rm.c, ulib.c`.
- no argument -> print an error message.
- convert a string to a integer by `atoi`.
- use system call `sleep(int n)`.
- see `kernel/sysproc.c`, `user/user.h`, `user/usys.S`.
- make sure `exit()`.
- add your sleep program to UPROGS in Makefile;


Q: There are 4 different 'sleep things' in different files, it's quitewerid. So what exactly happened when the code line `sleep(n)`(in sleep.c) was 'executed'?

- the  compiler looked up the `user.h` and found the signature `int sleep(int)`, so there was a function named `sleep`;
- and there was a `sleep` in the `usys.S` too, the assembler turned the `sleep.S` and `usys.S` into `sleep.o` as well as `usys.o`;
- the linker linked the code in `sleep.o` with other code it needed, like the code in some libraries. Then it would find that the code line (originally `sleep(n)`) in `sleep.o` pointed to a function named sleep, and there was just one in the  `usys.o`; So the link would link them together, and it just worked.

The point is that `usys.S` provides a way to get into the kernel, because only assembly language, instead of C, can do that.


Q: When executing `sleep.c`, how can the computer know `usys.S` which don't even appear in `sleep.c`.
- take a look at the Makefile.


