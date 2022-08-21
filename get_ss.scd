get_ss(1)

# NAME

get_ss - print segment selectors

# SYNOPSIS

*get_ss* [_SEGMENTS_SELECTORS_...]

# DESCRIPTION

*get_ss* prints information about the current segment selectors. It
will print information about every segment selectors if no _SEGMENTS_SELECTORS_
are provided, or print information the _SEGMENTS_SELECTORS_ provided as
arguments.

# EXAMPLE

```
$ get_ss cs ss
cs: 51 (index: 6, ti: 0, rpl: 3)
ss: 43 (index: 5, ti: 0, rpl: 3)
```
