bufpipe(1)

# NAME

bufpipe - buffer stdin

# SYNOPSIS

*bufpipe* _command_ [_args_]

# DESCRIPTION

*bufpipe* read  its standard input and write it to a temporary file. This
temporary file is a regular file and it becomes the standard input for
_command_. This allows to pipe to command that only read via mmap. An example of
its use is the following:

```
$ gcc bufpipe.c -o/dev/stdout | bufpipe readelf -h /dev/stdin
  Entry point address:               0x10f0
  Start of program headers:          64 (bytes into file)
  [...]
```
