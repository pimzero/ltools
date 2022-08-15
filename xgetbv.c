#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char** argv)
{
	uint32_t eax;
	uint32_t edx;
	uint32_t ecx = 0;

	const char* noargs[] = {
		"0",
		NULL,
	};

	if (argc == 1)
		argv = noargs;
	else
		argv++;

	do {
		ecx = atoi(*argv);
		argv++;
		__asm__ __volatile__("xgetbv\n\t"
				: "=a"(eax), "=d"(edx) : "c"(ecx));

		printf("ecx: 0x%x, eax: 0x%x, edx: 0x%x\n", ecx, eax, edx);
	} while (*argv);

	return 0;
}
