memfd_secret(1)

# NAME

memfd_secret - open a *memfd_secret*(2) fd for the provided command

# SYNOPSIS

*memfd_secret* _command_ [_arguments_...]

# DESCRIPTION

This command creates a *memfd_secret*(2) file and executes _command_ with the
provided _arguments_ while this file is open. The file descriptor for this file
will be the first available file descriptor number.
