set_tsc(1)

# NAME

set_tsc - cause SIGSEGV when the process tries to read timestamp counter

# SYNOPSIS

*set_tsc* _command_ [_args_...]++
*set_tsc*

# DESCRIPTION

*set_tsc* disable the capability of the process to read the timestamp counter,
and executes _command_ with the provided _args_. It does it by calling
*PR_SET_TSC* with *PR_TSC_SIGSEGV* as defined in *prctl*(2) man page.

If no argument is provided, *set_tsc* will print the value of the flag
determining whether the timestamp counter can be read or not.

# SEE ALSO

*prctl*(2)
