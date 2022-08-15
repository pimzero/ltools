#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	__asm__ __volatile__("vmcall\n\t"
			: :
			: "rcx", "rdx", "rsi" );
	return 0;
}
