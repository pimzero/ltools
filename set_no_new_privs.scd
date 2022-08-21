set_no_new_privs(1)

# NAME

set_no_new_privs - set the no_new_privs thread attribute

# SYNOPSIS

*set_no_new_privs* _command_ [_args_...]++
*set_no_new_privs*

# DESCRIPTION

*set_no_new_privs* sets the _no_new_privs_ thread attribute to 1 and executes
_command_ with the provided _args_, causing the behaviour defined in *prctl*(2)
for *PR_SET_NO_NEW_PRIVS*.

If no argument is provided, *set_no_new_privs* will print the value of the of
the *no_new_privs* attribute.

# SEE ALSO

*prctl*(2)
