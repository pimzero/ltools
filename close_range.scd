close_range(1)

# NAME

close_range - close a range of file descriptors

# SYNOPSIS

*close_range* _first_fd:last_fd_ [_first_fd:last_fd_...] _-- command_ [_args..._]

# DESCRIPTION

Close file descriptors in the range defined starting at _first_fd_ to _last_fd_
(included). Multiple range can be defined.

This will try to use *close_range*(2) if available, or *close*(3) as fallback.
