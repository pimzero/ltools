mmapable(1)

# NAME

mmapable - tell if a file supports the mmap file operation

# SYNOPSIS

*mmapable* _FILE_...

# DESCRIPTION

*mmapable* will try to *mmap*(2) each _FILE_ as read only, and for each file
will display if this operation succeeded (signaled by a 1 next to the file path)
or not (signaled by a 0). If it failed, the error reason provided by *errno*(3)
will be displayed as well on the standard error output.

# EXAMPLE

```
$ echo test >./file
$ mmapable . /dev/null ./file 
mmap: No such device
.: 0
mmap: No such device
/dev/null: 0
./file: 1
```
