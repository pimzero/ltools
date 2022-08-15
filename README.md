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
 - `cpuid`: pretty print information from the `cpuid` instruction
 - `dumpauxv`: pretty print a auxv file
 - `lsholes`: show holes/data in sparse files
 - `get_ss`: print segment selectors
 - `hpet`: get info about local hpet
 - `mem_cat`: dump memory of another process
 - `memfd_secret`: open a `memfd_secret` fd for the provided command
 - `mmapable`: tell if a file supports the mmap file operation
 - `rdpid`: read the processor ID with rdpid x86 instruction
 - `rdrand`: print random numbers generated with the `rdrand` instruction
 - `readcmos`: read the CMOS clock register
 - `readio`/`writeio`: read/write to an io port
 - `segm2linear`: convert an x86 real-mode segmented address to a linear address
 - `sgdt`: get the base and limit of `sgdt`, `sldt`, `sidt`, and `str`
   instructions
 - `smsw`: pretty print the result of the `smsw` instruction
 - `xgetbv`: pretty print the result of the `xgetbv` instruction
