#include <stdint.h>
#include <stdio.h>

__asm__(".global smsw\n"
"smsw:\n\t"
"smsw %ax\n\t"
"ret\n\t");

extern uint16_t smsw(void);

static void print_cr0(uint16_t cr0) {
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

int main() {
	print_cr0(smsw());
}
