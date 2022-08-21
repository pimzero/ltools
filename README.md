ltool: a set of simple tools
============================

This repository contains few tools that I wrote over the years to check
something once. They are kind of like my throw-away shell scripts, only in C
and for low-level stuff. They are not big enough nor important enough to have
their own repository, but still, they needed too much energy to stay in `/tmp`,
so here they are.

Those tools are mainly linux/x86 oriented.

 - [`bufpipe`: buffer stdin to be used as a regular file](#bufpipe)
 - [`close_range`: close a range of file descriptors](#close_range)
 - [`coredump_filter`: get/set `coredump_filter` flags](#coredump_filter)
 - [`cpuid`: pretty print information from the `CPUID` instruction](#cpuid)
 - [`dumpauxv`: pretty print a auxv file](#dumpauxv)
 - [`dumpvdso`: dump vDSO (wip)](#dumpvdso)
 - [`fxsave`: pretty print the result of `FXSAVE` x86 instruction](#fxsave)
 - [`getpid32`: print pid and wait](#getpid32)
 - [`getpid`: print self or parent pid](#getpid)
 - [`get_ss`: print segment selectors](#get_ss)
 - [`hpet`: get info about local hpet (wip)](#hpet)
 - [`lsholes`: show holes/data in sparse files](#lsholes)
 - [`mem_cat`: dump memory of another process (wip)](#mem_cat)
 - [`memfd_secret`: open a `memfd_secret` fd for the provided command](#memfd_secret)
 - [`mmapable`: tell if a file supports the `mmap` file operation](#mmapable)
 - [`parse_segm`: convert x86 segmented address to linear address](#parse_segm)
 - [`rdpid`: read the processor ID with `RDPID` x86 instruction](#rdpid)
 - [`rdrand`: print random numbers generated with the `RDRAND` instruction](#rdrand)
 - [`rdtsc`: print `RDTSC`](#rdtsc)
 - [`readcmos`: read the CMOS clock register](#readcmos)
 - [`readio`: read an io port](#readio)
 - [`segm2linear`: convert an x86 real-mode segmented address to a linear](#segm2linear)
   address
 - [`setcpuid`: enable or disable `CPUID` instruction](#setcpuid)
 - [`set_no_new_privs`: set the `no_new_privs` thread attribute](#set_no_new_privs)
 - [`set_ptracer`: set this process ptracer](#set_ptracer)
 - [`set_tsc`: cause SIGSEGV when the process tries to read timestamp counter](#set_tsc)
 - [`sgdt`: get the base and limit of `SGDT`, `SLDT`, `SIDT`, and `STR`](#sgdt)
   instructions
 - [`smsw`: pretty print the result of the `SMSW` instruction](#smsw)
 - [`speculation`: control the process speculation configuration](#speculation)
 - [`statvfs`: get filesystem statistics](#statvfs)
 - [`udpsend`: send something over UDP](#udpsend)
 - [`vmcall`: execute vmcall instruction in userland (fails)](#vmcall)
 - [`writeio`: write to an io port](#writeio)
 - [`x86_int`: execute `INT` instruction](#x86_int)
 - [`xgetbv`: pretty print the result of the `XGETBV` instruction](#xgetbv)

Dependencies
------------

 - `scdoc` for documentation
 - `pandoc` to update README.md

Utils
-----

### bufpipe

#### NAME

bufpipe - buffer stdin

#### SYNOPSIS

**bufpipe** *command* \[*args*\]

#### DESCRIPTION

**bufpipe** read its standard input and write it to a temporary file.
This temporary file is a regular file and it becomes the standard input
for *command*. This allows to pipe to command that only read via mmap.
An example of its use is the following:

    $ gcc bufpipe.c -o/dev/stdout | bufpipe readelf -h /dev/stdin
      Entry point address:               0x10f0
      Start of program headers:          64 (bytes into file)
      [...]

### close_range

#### NAME

close_range - close a range of file descriptors

#### SYNOPSIS

**close_range** *first_fd:last_fd* \[*first_fd:last_fd*\...\] *\--
command* \[*args\...*\]

#### DESCRIPTION

Close file descriptors in the range defined starting at *first_fd* to
*last_fd* (included). Multiple range can be defined.

This will try to use **close_range**(2) if available, or **close**(3) as
fallback.

### coredump_filter

#### NAME

coredump_filter - pretty print the content of /proc/pid/coredump_filter

#### SYNOPSIS

**coredump_filter** \[*-s BITSET*\] *-p pid*

**coredump_filter** *-s BITSET \-- commandline*

#### DESCRIPTION

Read the coredump_filter file for the given *pid*, or stdin if no *pid*
is given. If *-s* flag is set, write the *BITSET* to the *pid*
coredump_filter file.

    $ ./coredump_filter -p $$
    ANON_PRIVATE|ANON_SHARED|ELF_HEADERS|HUGETLB_PRIVATE
    $ ./coredump_filter -s 'ANON_PRIVATE|ANON_SHARED' -p $$
    $ ./coredump_filter -p $$
    ANON_PRIVATE|ANON_SHARED
    $ ./coredump_filter -p - < /proc/self/coredump_filter
    ANON_PRIVATE|ANON_SHARED
    $ ./coredump_filter -s 0x1ff -- cat /proc/self/coredump_filter
    000001ff
    $ kill -ABRT $$

### cpuid

#### NAME

cpuid - pretty print cpuid

#### SYNOPSIS

**cpuid** \[*leaf*\...\]

#### DESCRIPTION

Pretty prints the result of the **CPUID** x86 instruction if a parser
for the selected leaf is available. If no parser is available, the value
of the **eax**, **ebx**, **ecx**, and **edx** registers will be printed.

Running **cpuid** with no argument is the same as running \`cpuid 0\`.

    $ cpuid
    GenuineIntel
    0xD000000
    $ cpuid  1234 0x1234
    cpuid(0x000004D2): eax: 0x00000000, ebx: 0x00000000, ecx: 0x00000000, edx: 0x00000000
    cpuid(0x00001234): eax: 0x00000000, ebx: 0x00000000, ecx: 0x00000000, edx: 0x00000000

### dumpauxv

#### NAME

dumpauxv - dump auxv

#### SYNOPSIS

**dumpauxv** \[*auxv\...*\]

#### DESCRIPTION

**dumpauxv** reads the files given as argument (or its standard output
if none given) and pretty print their content as if they were auxval
array.

    $ dumpauxv /proc/self/auxv
      SYSINFO_EHDR:   0x7ffe09774000
      HWCAP:  0xbfebfbff
      PAGESZ: 4096
      [...]

### fxsave

#### NAME

fxsave - pretty print the output of FXSAVE instruction

#### SYNOPSIS

**fxsave** \[**-mxre**\] \[**-p** *hex*\] \[**-c** *hex_code*\]

#### DESCRIPTION

**fxsave** prints the output of the **FXSAVE** x86 instruction.

#### OPTIONS

**-h**

> Print the help summary.

**-c** *code*

> Provide x86 code to execute before the call to **FXSAVE**. This code
> is to be provided in hexadecimal and will be executed directly with no
> checks.
>
> This will be executed as its own function, and a **RET** (0xc3) is
> automatically appended to this code.

**-e**

> Execute **FXSAVE** instruction with **rex.w** prefix set to 1
> (extended 64 bit mode). The output is then following the layout of the
> 64-bit mode FXSAVE64 Map, instead of FXSAVE Map, as documented on the
> Intel SDM Volume 2.
>
> This option is not available if **fxsave** was built in 32bit mode.

**-p** *pattern*

> Initialization pattern of **FXSAVE** area. *pattern* is an hexadecimal
> string. The default is 0x5a. It is useful to find what is part of the
> memory is overridden by **FXSAVE**. Note that it is possible that a
> register is filled with the pattern. It may be useful to clear the
> register with the **-c** option in this case.

**-m**, **-x**, **-r**

> Toggle printing MM registers (**-m**), XMM registers (**-x**), and
> reserved fields (**-r**). By default, they are not shown.

### getpid

#### NAME

> getpid - print current or parent PID

#### SYNOPSIS

**getpid** \[**-p**\]

#### DESCRIPTION

This prints the current process PID, or the parent process PID if **-p**
flag is provided.

**-p**

> Print the parent PID

### getpid32

#### NAME

getpid32 - print current pid and wait

#### SYNOPSIS

**getpid32**

#### DESCRIPTION

This program prints its pid and waits. It must be killed with a signal.
It is built with \`-m32\` compilation option.

### get_ss

#### NAME

get_ss - print segment selectors

#### SYNOPSIS

**get_ss** \[*SEGMENTS_SELECTORS*\...\]

#### DESCRIPTION

**get_ss** prints information about the current segment selectors. It
will print information about every segment selectors if no
*SEGMENTS_SELECTORS* are provided, or print information the
*SEGMENTS_SELECTORS* provided as arguments.

#### EXAMPLE

    $ get_ss cs ss
    cs: 51 (index: 6, ti: 0, rpl: 3)
    ss: 43 (index: 5, ti: 0, rpl: 3)

### lsholes

#### NAME

lsholes - list holes in sparse files

#### SYNOPSIS

**lsholes** \[**-cx**\] *FILES\...*

#### DESCRIPTION

**lsholes** will show information about data and holes in files, and in
particular in sparse files.

**-c**

> Print a summary with data & hole size rather than 1 line per
> contiguous blocks of data/hole.

**-x**

> Show numeric output in hexadecimal.

#### EXAMPLE

    $ truncate -s 512M file
    $ echo 'not hole' >>file
    $ lsholes file
    /tmp/block:
    hole: 0-536870912
    data: 536870912-536870922

### memfd_secret

#### NAME

memfd_secret - open a **memfd_secret**(2) fd for the provided command

#### SYNOPSIS

**memfd_secret** *command* \[*arguments*\...\]

#### DESCRIPTION

This command creates a **memfd_secret**(2) file and executes *command*
with the provided *arguments* while this file is open. The file
descriptor for this file will be the first available file descriptor
number.

### mmapable

#### NAME

mmapable - tell if a file supports the mmap file operation

#### SYNOPSIS

**mmapable** *FILE*\...

#### DESCRIPTION

**mmapable** will try to **mmap**(2) each *FILE* as read only, and for
each file will display if this operation succeeded (signaled by a 1 next
to the file path) or not (signaled by a 0). If it failed, the error
reason provided by **errno**(3) will be displayed as well on the
standard error output.

#### EXAMPLE

    $ echo test >./file
    $ mmapable . /dev/null ./file 
    mmap: No such device
    .: 0
    mmap: No such device
    /dev/null: 0
    ./file: 1

### parse_segm

#### NAME

parse_segm - convert x86 segmented address to linear address

#### SYNOPSIS

**parse_segm** *SEGMENT:OFFSET*\...

#### DESCRIPTION

Convert x86 real mode *SEGMENT:OFFSET* address to a linear address. This
is naively the result of *SEGMENT* \* 0x10 + *OFFSET*.

#### EXAMPLE

    $ parse_segm 7eb:9bf 1a2:21a
    7eb:9bf: 0x886f
    1a2:21a: 0x1c3a

### rdpid

#### NAME

rdpid - print the processor ID provided by the RDPID instruction

#### SYNOPSIS

**rdpid**

#### DESCRIPTION

Execute the RDPID x86 instruction and print its result (in the RAX
register) to the standard output.

### rdrand

#### NAME

rdrand - print random numbers generated with the RDRAND instruction

#### SYNOPSIS

**rdrand**

#### DESCRIPTION

Output the result of the **RDRAND** instruction in an infinite loop. To
exit a signal needs to be sent to **rdrand**.

#### EXAMPLE

    $ rdrand | xxd
    00000000: fac2 5478 9e57 332d 626a 58db 03be 82ce  ..Tx.W3-bjX.....
    00000010: 285b d948 7328 158d adc5 2f67 7ea1 5e07  ([.Hs(..../g~.^.
    00000020: b6b2 d631 529e f961 b91e 6d7f 45c0 6362  ...1R..a..m.E.cb
    [...]
    ^C

### rdtsc

#### NAME

rdtsc - print the Time Stamp Counter provided by the RDTSC instruction

#### SYNOPSIS

**rdtsc**

#### DESCRIPTION

**rdtsc** prints the Time Stamp Counter provided by the **RDTSC** x86
instruction.

### readcmos

#### NAME

readcmos - read the CMOS clock register

#### SYNOPSIS

**readcmos** \[*register*\...\]

#### DESCRIPTION

Print the selected *register* of the CMOS clock (IO Ports 0x70 & 0X71).
By default, the register 0xd is selected.

### readio

#### NAME

readio - read an io port

#### SYNOPSIS

**readio** \[*PORT*\...\]

#### DESCRIPTION

**readio** read the IO ports specified by the *PORT* arguments. This
needs the */dev/port* file to be present.

#### EXAMPLE

    # sudo readio 0xb2
    0xb2: 0xf5

### seg2linear

#### NAME

seg2linear - convert x86 segmented address to linear address

#### SYNOPSIS

**seg2linear** *SEGMENT:OFFSET*\...

#### DESCRIPTION

Convert x86 real mode *SEGMENT:OFFSET* address to a linear address. This
is naively the result of *SEGMENT* \* 0x10 + *OFFSET*.

#### EXAMPLE

    $ seg2linear 7eb:9bf 1a2:21a
    7eb:9bf = 0x886f
    1a2:21a = 0x1c3a

### setcpuid

#### NAME

setcpuid - enable or disable CPUID instruction

#### SYNOPSIS

**setcpuid** **setcpuid** *enable* *command* \[*args*\...\]

#### DESCRIPTION

**setcpuid** allows to enable/disable the CPUID instruction.

If no argument are provided, **setcpuid** will print 0 if CPUID is
disabled or another value if it is enabled.

### set_no_new_privs

#### NAME

set_no_new_privs - set the no_new_privs thread attribute

#### SYNOPSIS

**set_no_new_privs** *command* \[*args*\...\]\
**set_no_new_privs**

#### DESCRIPTION

**set_no_new_privs** sets the *no_new_privs* thread attribute to 1 and
executes *command* with the provided *args*, causing the behaviour
defined in **prctl**(2) for **PR_SET_NO_NEW_PRIVS**.

If no argument is provided, **set_no_new_privs** will print the value of
the of the **no_new_privs** attribute.

#### SEE ALSO

**prctl**(2)

### set_ptracer

#### NAME

set_ptracer - set this process ptracer

#### SYNOPSIS

**set_ptracer** \[-**p** *tracer*\] *command* \[*arguments*\...\]

#### DESCRIPTION

**set_ptracer** has the behaviour of calling **PR_SET_PTRACER**
(documented in **prctl**(2)) with the *tracer* argument as tracer
process ID, or **PR_SET_PTRACER_ANY** if no *tracer* argument is
provided, then executing the *command* with the provided *arguments*.

#### SEE ALSO

**prctl**(2)

### set_tsc

#### NAME

set_tsc - cause SIGSEGV when the process tries to read timestamp counter

#### SYNOPSIS

**set_tsc** *command* \[*args*\...\]\
**set_tsc**

#### DESCRIPTION

**set_tsc** disable the capability of the process to read the timestamp
counter, and executes *command* with the provided *args*. It does it by
calling **PR_SET_TSC** with **PR_TSC_SIGSEGV** as defined in
**prctl**(2) man page.

If no argument is provided, **set_tsc** will print the value of the flag
determining whether the timestamp counter can be read or not.

#### SEE ALSO

**prctl**(2)

### sgdt

#### NAME

sgdt - get the base and limit of SGDT, SLDT, SIDT, and STR instructions

#### SYNOPSIS

**sgdt** \[*descriptor*\...\]

#### DESCRIPTION

**sgdt** prints informations about the x86 registers related to x86
descriptor tables. The registers shown depend on the *descriptor*
argument:

> ·
>
> **gdt** will print the Global Descriptor Table Register (GDTR)

> ·
>
> **ldt** will print the Local Descriptor Table Register (LDTR)

> ·
>
> **idt** will print the Interrupt Descriptor Table Register (IDTR)

> ·
>
> **tr** will print the Task Register (TR)

If no argument is provided, **sgdt** behaves as if \"**gdt**\" was
provided as *descriptor*.

### smsw

#### NAME

smsw - pretty print the result of the SMSW instruction

#### SYNOPSIS

**smsw**

#### DESCRIPTION

Print the content of the machine status word (bits 0-15 of CR0) provided
by the SMSW x86 instruction.

### speculation

#### NAME

speculation - control the process speculation configuration

#### SYNOPSIS

**speculation**

#### DESCRIPTION

### statvfs

#### NAME

statvfs - get filesystem statistics

#### SYNOPSIS

**statvfs** *mountpoint*\...

#### DESCRIPTION

**statvfs** prints on the standard output the result of the
**statvfs**(2) syscall for the specified *mountpoint*.

### udpsend

#### NAME

udpsend - send something over UDP

#### SYNOPSIS

**udpsend** *address* *port*

#### DESCRIPTION

**udpsend** sends one UDP packet to the destination provided by
*address* and *port*. The payload of the packet is given on the standard
input.

### vmcall

#### NAME

vmcall - execute vmcall instruction in userland

#### SYNOPSIS

**vmcall**

#### DESCRIPTION

Call the **vmcall** instruction from userland. Hopefully cause a SIGILL.

### writeio

#### NAME

writeio - write to an io port

#### SYNOPSIS

**writeio** *PORT* *VALUE*

#### DESCRIPTION

**writeio** writes the *VALUE* to the IO port specifed by the *PORT*
argument. This needs the */dev/port* file to be present.

### x86_int

#### NAME

x86_int: execute the x86 INT instruction

#### SYNOPSIS

**x86_int** \[**rax**\|**rbx**\|**rcx**\...=*val*\]\... *int_nr*
\[\[**rax**\|**rbx**\|**rcx**\...=*val*\]\... *int_nr*\]\...

#### DESCRIPTION

**x86_int** executes a x86 INT instruction. *int_rn* is the interrupt
number used for the INT instruction. the registers are set to the value
defined by the arguments, or are left undefined if they are not
explicitely set.

It will use the 2 byte encoding of the instruction for every *int_nr*
argument (it will not use the INT3 or INT1 instructions).

#### EXAMPLE

Call fork (INT 0x80 with rax=2) and then a breakpoint (INT 0x3):

    $ x86_int rax=2 0x80 3
    rax=0x2
    int $0x80
    int $0x3
    int $0x3
    Trace/breakpoint trap (core dumped)

### xgetbv

#### NAME

xgetbv - pretty print the result of the XGETBV instruction

#### SYNOPSIS

**xgetbv** \[*ecx*\]\...

#### DESCRIPTION

Print the content of the extended control register specified by the
*ecx* argument.

If no argument is provided, **xgetbv** behaves as if it was called with
0, displaying the content of XCR0.
