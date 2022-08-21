x86_int(1)

# NAME

x86_int: execute the x86 INT instruction

# SYNOPSIS

*x86_int* [*rax*|*rbx*|*rcx*...=_val_]... _int_nr_
\[[*rax*|*rbx*|*rcx*...=_val_]... _int_nr_]...

# DESCRIPTION

*x86_int* executes a x86 INT instruction. _int_rn_ is the interrupt number used
for the INT instruction. the registers are set to the value defined by the
arguments, or are left undefined if they are not explicitely set.

It will use the 2 byte encoding of the instruction for every _int_nr_ argument
(it will not use the INT3 or INT1 instructions).

# EXAMPLE

Call fork (INT 0x80 with rax=2) and then a breakpoint (INT 0x3):

```
$ x86_int rax=2 0x80 3
rax=0x2
int $0x80
int $0x3
int $0x3
Trace/breakpoint trap (core dumped)
```
