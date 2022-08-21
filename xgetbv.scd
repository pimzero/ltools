xgetbv(1)

# NAME

xgetbv - pretty print the result of the XGETBV instruction

# SYNOPSIS

*xgetbv* [_ecx_]...

# DESCRIPTION

Print the content of the extended control register specified by the _ecx_
argument.

If no argument is provided, *xgetbv* behaves as if it was called with 0,
displaying the content of XCR0.
