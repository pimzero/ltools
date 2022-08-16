coredump_filter(1)

# NAME

coredump_filter - pretty print the content of /proc/pid/coredump_filter

# SYNOPSIS

*coredump_filter* [_-s BITSET_] _-p pid_

*coredump_filter* _-s BITSET -- commandline_

# DESCRIPTION

Read the coredump_filter file for the given _pid_, or stdin if no _pid_ is
given. If _-s_ flag is set, write the _BITSET_ to the _pid_ coredump_filter
file.

```
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
```
