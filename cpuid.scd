cpuid(1)

# NAME

cpuid - pretty print cpuid

# SYNOPSIS

*cpuid* [_leaf_...]

# DESCRIPTION

Pretty prints the result of the *CPUID* x86 instruction if a parser for the
selected leaf is available. If no parser is available, the value of the *eax*,
*ebx*, *ecx*, and *edx* registers will be printed.

Running *cpuid* with no argument is the same as running `cpuid 0`.

```
$ cpuid
GenuineIntel
0xD000000
$ cpuid  1234 0x1234
cpuid(0x000004D2): eax: 0x00000000, ebx: 0x00000000, ecx: 0x00000000, edx: 0x00000000
cpuid(0x00001234): eax: 0x00000000, ebx: 0x00000000, ecx: 0x00000000, edx: 0x00000000
```
