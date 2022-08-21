fxsave(1)

# NAME

fxsave - pretty print the output of FXSAVE instruction

# SYNOPSIS

*fxsave* [*-mxre*] [*-p* _hex_] [*-c* _hex_code_]

# DESCRIPTION

*fxsave* prints the output of the *FXSAVE* x86 instruction.

# OPTIONS

*-h*
	Print the help summary.

*-c* _code_
	Provide x86 code to execute before the call to *FXSAVE*. This code is to
	be provided in hexadecimal and will be executed directly with no checks.

	This will be executed as its own function, and a *RET* (0xc3) is
	automatically appended to this code.

*-e*
	Execute *FXSAVE* instruction with *rex.w* prefix set to 1 (extended 64
	bit mode). The output is then following the layout of the 64-bit mode
	FXSAVE64 Map, instead of FXSAVE Map, as documented on the Intel SDM
	Volume 2.

	This option is not available if *fxsave* was built in 32bit
	mode.

*-p* _pattern_
	Initialization pattern of *FXSAVE* area. _pattern_ is an hexadecimal
	string. The default is 0x5a. It is useful to find what is part of the
	memory is overridden by *FXSAVE*. Note that it is possible that a
	register is filled with the pattern. It may be useful to clear the
	register with the *-c* option in this case.

*-m*, *-x*, *-r*
	Toggle printing MM registers (*-m*), XMM registers (*-x*), and reserved
	fields (*-r*). By default, they are not shown.
