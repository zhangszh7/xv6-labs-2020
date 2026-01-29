# Lab Utilities

- [x]  Boot xv6
- [x]  sleep
- [x]  pingpong  
- [x]  primes 
- [x]  find
- [x]  xargs
- [ ]  optional

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


Q: There are 4 different 'sleep things' in different files,
it's quite werid.
So what exactly happened when the code line `sleep(n)`(in sleep.c) was 'executed'?

- the  compiler looked up the `user.h` and found the signature `int sleep(int)`, so there was a function named `sleep`;
- and there was a `sleep` in the `usys.S` too, the assembler turned the `sleep.S` and `usys.S` into `sleep.o` as well as `usys.o`;
- the linker linked the code in `sleep.o` with other code it needed, like the code in some libraries.
Then it would find that the code line (originally `sleep(n)`) in `sleep.o` pointed to a function named sleep,
and there was just one in the  `usys.o`; So the linker would link them together, and it just worked.
The point is that `usys.S` provides a way to get into the kernel,
because only assembly language, instead of C, can do that.


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
- the paper is talking about `a natural abstraction the can make programs much simpler`,
instead of achieving high performance.
- the prime sieve: an CSP case. one process eliminates one prime.

hints:
- pay attention to fd, close it in time.
- using `wait(0)`.
- read returns zero when the write-side of a pipe is closed. 
- It's simplest to directly write 32-bit (4-byte) ints to the pipes, rather than using formatted ASCII I/O.
- add to `UPROGS` in the Makefile.

improvment:
- simplify the logic, make use of the abstraction.
- add comments, make the code easy to read.
- version 1.0: iteration (all in one)  ->  version 2.0: recursion
(more)
- Guard Clauses: (for code style)
`"Return early, fail fast"`, inspect errors first with just `if` statements.

## find

target:
- find all the files in a directory tree with a specific name. 

hints:
- `ls.c`: how to read directories.
- use recursion.
- don't recurse into `.` and `..`.
- read C book, learn how to manipulat a string.
- use `make clean` to delete fs.img(the virtual disk) if the file system fails.
- `strcmp()`: compare strings.
- UPROGS

improvment:
(although the code passed the test in just one run, there were some latent problems which I didn't notice.)
- `fd`: when I opened a file and used `If-return` to check errors, I just returned without `close(fd)`.
- `T_DEVICE` also needs to consider.
- consider that users possibly pass a file name as the top directory.

## xargs

target:
- read lines from the standard input and run a command for each line.  

hints:
- `fork` and `exec`.
- read a character at a time until `\n`.
- `kernel/param.h`: MAXARG 32
- UPROGS
 
improvment:
- figure out the problem before writing the implemention.
- be careful with the array and pointer, check errors if needed.
- the last element of argv[] must be `0`;
- think about all the possible inputs, such as the last line ends without `'\n'`.


- Functions that return a boolean (true or false) value should be named so that the return value is unambiguous. 


## optional challenges

waiting ...


## conclusion

The entire lab took me almost 20 hours to finish. During the whole time, 
I realised my misunderstandings of much konwledge and learned a lot, 
when I couldn't solve the problems or had to improve my implementions again and again.

1. write code
- to solve a problem step by step, 
you need to understand the entire problem enough before writing any code,
otherwise you will lose your mind and find your code just messy.

- to help yourself understand the problem, you can write some pseudocode, 
draw a picture and speak out(explain) your thinking loudly,
or even better just `try some concrete examples`.

- check all the possible errors and print descriptive messages.(`fprintf`)
- obey the idioms and make your code consistent. (`if-else if-else` instead of `switch`)
- `guard clause`: return early, fail fast.
- free those resources that you won't need, like `fd`.
- choose reasonable names for your variables.

2. C language
- be careful with pointer and array, especially when you're allocating these things.
- `p[i]` is equivalent to `*(p + i)`, so `i[p]` is valid too!.

3. xv6
- the process of running a soure code. (what wiil a compilier or a linker do?, which I have not figured out totally)
- the difference between `exit()` and `return <>`.
- `thinking in a file view`: open -> fd -> stat -> read the content, no matter it's a real file, a directory or a pipe.
By the way, a directory file contains a sequence of dirent structures(include an inum and a name array),
`inmu == 0` means the dirent is invalid.

4. tools
- `Git`: a distributed version control system.
- `gcc`: make your code to be an executable file.
- `script`: record the interactive process into `typescript` including input and output.  
- `Makefile`











