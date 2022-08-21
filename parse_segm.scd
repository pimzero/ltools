parse_segm(1)

# NAME

parse_segm - convert x86 segmented address to linear address

# SYNOPSIS

*parse_segm* _SEGMENT:OFFSET_...

# DESCRIPTION

Convert x86 real mode _SEGMENT:OFFSET_ address to a linear address. This is
naively the result of _SEGMENT_ \* 0x10 + _OFFSET_.

# EXAMPLE

```
$ parse_segm 7eb:9bf 1a2:21a
7eb:9bf: 0x886f
1a2:21a: 0x1c3a
```
