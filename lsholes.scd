lsholes(1)

# NAME

lsholes - list holes in sparse files

# SYNOPSIS

*lsholes* [*-cx*] _FILES..._

# DESCRIPTION

*lsholes* will show information about data and holes in files, and in particular
in sparse files.

*-c*
	Print a summary with data & hole size rather than 1 line per contiguous
	blocks of data/hole.

*-x*
	Show numeric output in hexadecimal.

# EXAMPLE

```
$ truncate -s 512M file
$ echo 'not hole' >>file
$ lsholes file
/tmp/block:
hole: 0-536870912
data: 536870912-536870922
```
