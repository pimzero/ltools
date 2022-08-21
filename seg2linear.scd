seg2linear(1)

# NAME

seg2linear - convert x86 segmented address to linear address

# SYNOPSIS

*seg2linear* _SEGMENT:OFFSET_...

# DESCRIPTION

Convert x86 real mode _SEGMENT:OFFSET_ address to a linear address. This is
naively the result of _SEGMENT_ \* 0x10 + _OFFSET_.

# EXAMPLE

```
$ seg2linear 7eb:9bf 1a2:21a
7eb:9bf = 0x886f
1a2:21a = 0x1c3a
```
