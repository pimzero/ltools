ltool: a set of simple tools
============================

This repository contains few tools that I wrote over the years to check
something once. They are kind of like my throw-away shell scripts, only in C
and for low-level stuff. They are not big enough nor important enough to have
their own repository, but still, they needed too much energy to stay in `/tmp`,
so here they are.

Those tools are mainly linux/x86 oriented.

 - `bufpipe`: buffer stdin to be used as a regular file
 - `close_range`: close a range of file descriptors
 - `coredump_filter`: get/set `coredump_filter` flags
 - `cpuid`: pretty print information from the `CPUID` instruction
 - `dumpauxv`: pretty print a auxv file
 - `dumpvdso`: dump vDSO (wip)
 - `fxsave`: pretty print the result of `FXSAVE` x86 instruction
 - `getpid32`: print pid and wait
 - `getpid`: print self or parent pid
 - `get_ss`: print segment selectors
 - `hpet`: get info about local hpet (wip)
 - `lsholes`: show holes/data in sparse files
 - `mem_cat`: dump memory of another process (wip)
 - `memfd_secret`: open a `memfd_secret` fd for the provided command
 - `mmapable`: tell if a file supports the `mmap` file operation
 - `parse_segm`: convert x86 segmented address to linear address
 - `rdpid`: read the processor ID with `RDPID` x86 instruction
 - `rdrand`: print random numbers generated with the `RDRAND` instruction
 - `rdtsc`: print `RDTSC`
 - `readcmos`: read the CMOS clock register
 - `readio`: read an io port
 - `segm2linear`: convert an x86 real-mode segmented address to a linear
   address
 - `setcpuid`: enable or disable `CPUID` instruction
 - `set_no_new_privs`: set the `no_new_privs` thread attribute
 - `set_ptracer`: set this process ptracer
 - `set_tsc`: cause SIGSEGV when the process tries to read timestamp counter
 - `sgdt`: get the base and limit of `SGDT`, `SLDT`, `SIDT`, and `STR`
   instructions
 - `smsw`: pretty print the result of the `SMSW` instruction
 - `speculation`: control the process speculation configuration
 - `statvfs`: get filesystem statistics
 - `udpsend`: send something over UDP
 - `vmcall`: execute vmcall instruction in userland (fails)
 - `writeio`: write to an io port
 - `x86_int`: execute `INT` instruction
 - `xgetbv`: pretty print the result of the `XGETBV` instruction

Dependencies
------------

 - `scdoc` for documentation
