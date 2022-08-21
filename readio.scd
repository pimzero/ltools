readio(1)

# NAME

readio - read an io port

# SYNOPSIS

*readio* [_PORT_...]

# DESCRIPTION

*readio* read the IO ports specified by the _PORT_ arguments. This needs the
_/dev/port_ file to be present.

# EXAMPLE

```
# sudo readio 0xb2
0xb2: 0xf5
```
