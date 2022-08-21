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
