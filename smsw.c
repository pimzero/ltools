#include <stdio.h>

typedef short cr;

static void smws(cr* cr0)
{
	__asm__ __volatile__("smsw %0\n\t"
			     : : "m" (*cr0));
}

static void print_cr0(cr cr0)
{
	printf("CR0:");
#define X(Bit, Name) do { if (cr0 & (1 << (Bit))) printf(" " #Name); } while (0)
	X(0, PE);
	X(1, MP);
	X(2, EM);
	X(3, TS);
	X(4, ET);
	X(5, NE);
#undef X
	printf("\n");
}

int main()
{
	cr cr0 = 0;
	smws(&cr0);
	print_cr0(cr0);
}
