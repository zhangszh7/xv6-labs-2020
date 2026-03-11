# Lab: traps
- [x]   riscv assembly
- [x]   backtrace
- [x]   alarm
- [ ]   optional

# Preparation
- chapter4
- kernel/riscv.h
- kernel/trampoline.S
- kernel/trap.c

---

`ecall`:
save its own address in `sepc` by h/w,
save the current mode in `SPP` of `sstatus`,
save previous interrupt level in `SPIE` of `sstatus`,
turn off the `SIE`,
turn into `S-mode` from `U-mode`,
jump to where `stvec` points to. (`stvec` points to `uservec`)

`sret`:
restore `SIE` from `SPIE`,
turn back to previous mode(from `SPP`),
jump to where `sepc` points to.


`uservec`:
1. swap `a0` with `sscratch`, save user registers in TRAPFRAME.
   trapframe: 0~39(kernel sp/satp, epc ...), 40~288(registers).
2. restore kernel-related things, load kernel pagetable and refresh TLB.
3. jump to `usertrap()`.


`usertrap()`:
1. write `kernelvec` to `stvec`,
2. save user `pc` in `p->trapframe->epc`,
3. `scause`,
   if syscall, `epc + 4` points to next instruction(`ecall` -> `ret`);
   if devintr, ok;
   else `unexpected scause`.
4. call `usertrapret()`.


`usertrapret`:
1. `intr_off()`,
2. set up trapframe values,
3. set up registers `sret` will use, `sepc` and `sstatus`,
   `MAKE_SATP()`:
   (63~60:MODE = Sv39), (59~44:ASID = 0), (43~0:PPN >> 12 to ignore 0).
4. call `userret(TRAPFRAME, satp)`.


`userret`:
1. switch pgtbl,
2. restore user register,
3. `sret`.

---

## backtrace

- add the prototype in `kernel/defs.h`.  
- add `r_fp()` in `kernel/riscv.h`.
- add `backtrace()` in `kernel/printf.c`.


## alarm 

- syscall: `sigalarm(interval, handler)`.
- `user/alarmtest.c`.
- `user/user.h`.
- stubs, syscall.

- modify `proc.h` and initialize proc in `kernel/proc.c`.
  `p->ticks`, `p->handler`, `p->timer`.
- modify `sys_sigalarm()` in `kernel/sysproc.c`.
- modify `usertrap()`.

pass test0

- `struct trapframe *snapshot`.
- `enum handlerstate hstate`. `DURING, FREE`.
- modify `usertrap()`.
- modify `proc.c`.
- modify `sys_sigreturn()`.



- make CPUS=1 qemu-gdb













