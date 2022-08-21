rdrand(1)

# NAME

rdrand - print random numbers generated with the RDRAND instruction

# SYNOPSIS

*rdrand*

# DESCRIPTION

Output the result of the *RDRAND* instruction in an infinite loop. To exit a
signal needs to be sent to *rdrand*.

# EXAMPLE

```
$ rdrand | xxd
00000000: fac2 5478 9e57 332d 626a 58db 03be 82ce  ..Tx.W3-bjX.....
00000010: 285b d948 7328 158d adc5 2f67 7ea1 5e07  ([.Hs(..../g~.^.
00000020: b6b2 d631 529e f961 b91e 6d7f 45c0 6362  ...1R..a..m.E.cb
[...]
^C
```
