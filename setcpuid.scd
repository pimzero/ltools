setcpuid(1)

# NAME

setcpuid - enable or disable CPUID instruction

# SYNOPSIS

*setcpuid*
*setcpuid* _enable_ _command_ [_args_...]

# DESCRIPTION

*setcpuid* allows to enable/disable the CPUID instruction.

If no argument are provided, *setcpuid* will print 0 if CPUID is disabled or
another value if it is enabled.
