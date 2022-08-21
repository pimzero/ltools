sgdt(1)

# NAME

sgdt - get the base and limit of SGDT, SLDT, SIDT, and STR instructions

# SYNOPSIS

*sgdt* [_descriptor_...]

# DESCRIPTION

*sgdt* prints informations about the x86 registers related to x86 descriptor
tables. The registers shown depend on the _descriptor_ argument:

- *gdt* will print the Global Descriptor Table Register (GDTR)
- *ldt* will print the Local Descriptor Table Register (LDTR)
- *idt* will print the Interrupt Descriptor Table Register (IDTR)
- *tr* will print the Task Register (TR)

If no argument is provided, *sgdt* behaves as if "*gdt*" was provided as
_descriptor_.
