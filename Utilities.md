# Lab Utilities

- [x]  Boot xv6
- [x]  sleep
- [x]  pingpong  
- [x]  primes 
- [ ]  

## Boot xv6

- read the book 'progit', make a deep learning of Git.

## sleep

hints:
- look some programs in user/, like `echo.c, grep.c, rm.c, ulib.c`.
- no argument -> print an error message.
- convert a string to a integer by `atoi`.
- use system call `sleep(int n)`.
- see `kernel/sysproc.c`, `user/user.h`, `user/usys.S`.
- make sure `exit()`.
- add your sleep program to `UPROGS` in Makefile;


Q: There are 4 different 'sleep things' in different files, it's quite werid. So what exactly happened when the code line `sleep(n)`(in sleep.c) was 'executed'?

- the  compiler looked up the `user.h` and found the signature `int sleep(int)`, so there was a function named `sleep`;
- and there was a `sleep` in the `usys.S` too, the assembler turned the `sleep.S` and `usys.S` into `sleep.o` as well as `usys.o`;
- the linker linked the code in `sleep.o` with other code it needed, like the code in some libraries. Then it would find that the code line (originally `sleep(n)`) in `sleep.o` pointed to a function named sleep, and there was just one in the  `usys.o`; So the link would link them together, and it just worked.
The point is that `usys.S` provides a way to get into the kernel, because only assembly language, instead of C, can do that.


Q: When executing `sleep.c`, how can the computer know `usys.S` which don't even appear in `sleep.c`.
- take a look at the Makefile.

## pingpong

target:
- pingpong a byte over a pipe.
- print "<pid>:received ping/pong".

hints:
- `int fork()`, `int pipe(int p[])`, `int read(int fd, char *buf, int n)`, `int write(int fd, char *buf, int n)`. 
- `getpid()`. 
- add pingpong program to `UPROGS` in Makefile.
- library functions: `user.h`, `ulib.c`, `printf.c`, `umalloc.c`.

improvment:
- inspect and print error. 
- close fd when you're done.

## primes

target:( read [the page](https://swtch.com/~rsc/thread/)
- the paper is talking about `a natural abstraction the can make programs much simpler`, instead of achieving high performance.
- the prime sieve: an CSP case. one process eliminates one prime.

hints:
- pay attention to fd, close it in time.
- using `wait(0)`.
- read returns zero when the write-side of a pipe is closed. 
- It's simplest to directly write 32-bit (4-byte) ints to the pipes, rather than using formatted ASCII I/O.
- add to `UPROGS` in the Makefile.
























