set_ptracer(1)

# NAME

set_ptracer - set this process ptracer

# SYNOPSIS

*set_ptracer* [-*p* _tracer_] _command_ [_arguments_...]

# DESCRIPTION

*set_ptracer* has the behaviour of calling *PR_SET_PTRACER* (documented in
*prctl*(2)) with the _tracer_ argument as tracer process ID, or
*PR_SET_PTRACER_ANY* if no _tracer_ argument is provided, then executing the
_command_ with the provided _arguments_.

# SEE ALSO

*prctl*(2)
