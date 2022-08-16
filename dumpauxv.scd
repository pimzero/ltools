dumpauxv(1)

# NAME

dumpauxv - dump auxv

# SYNOPSIS

*dumpauxv* [_auxv..._]

# DESCRIPTION

*dumpauxv* reads the files given as argument (or its standard output if none
given) and pretty print their content as if they were auxval array.

```
$ dumpauxv /proc/self/auxv
  SYSINFO_EHDR:   0x7ffe09774000
  HWCAP:  0xbfebfbff
  PAGESZ: 4096
  [...]
```
