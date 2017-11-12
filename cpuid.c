#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define arr_sze(X) (sizeof(X) / sizeof(*(X)))

#if __x86_64__
#define REG "r"
#else
#define REG "e"
#endif

static uint32_t swap_endianness(uint32_t in)
{
	union { uint32_t val; uint8_t c[4]; } tmp;
	union { uint32_t val; uint8_t c[4]; } out;

	tmp.val = in;
	out.c[0] = tmp.c[3];
	out.c[1] = tmp.c[2];
	out.c[2] = tmp.c[1];
	out.c[3] = tmp.c[0];

	return out.val;
}

#define W(U32) (swap_endianness(*((uint32_t*)(U32))))
#define S(U32, Idx) ((uint16_t) ((W(U32) >> ((Idx) * 16)) & 0xffff))
#define B(U32, Idx) (*(((uint8_t*)(U32)) + (Idx)))

#define EAX ((char*)(val.i + 3))
#define EBX ((char*)(val.i + 0))
#define ECX ((char*)(val.i + 2))
#define EDX ((char*)(val.i + 1))

#define H8 "0x%02hhX"
#define H32 "0x%08X"

#define BE(X) (swap_endianness(X))

int main(int argc, char** argv)
{
	static union {
		uint8_t c[16];
		uint32_t i[4];
	} val;

	uint32_t cpuid_arg = 0;

	argv++;

	if (argc <= 1)
		goto default_opt;

	while (*argv) {
		cpuid_arg = strtoul(*argv, NULL, 0);
		argv++;
default_opt:
		__asm__ __volatile__("mov %%" REG "bx, %%" REG "si\n\t"
				"cpuid\n\t"
				"mov %%ebx, (%%" REG "si)\n\t"
				"mov %%edx, 4(%%" REG "si)\n\t"
				"mov %%ecx, 8(%%" REG "si)\n\t"
				"mov %%eax, 12(%%" REG "si)\n\t"
				: : "a"(cpuid_arg), "b" (&val)
				: "ecx", "edx", REG "si" );

		switch (cpuid_arg) {
		case 0x0:
			printf("%.12s\n0x%X\n", val.c, BE(val.i[3]));
			break;
		case 0x1:
			printf(
			"Version Information: " H32 "\n"
			"Brand Index: " H8 "\n"
			"CLFLUSH line size: " H8 "\n"
			"Number of logical processor per physical core: " H8 "\n"
			"Local APIC ID: " H8 "\n"
			"Extended Feature informations: " H32 "\n"
			"Feature Information: " H32 "\n",
			W(EAX), B(EBX, 0), B(EBX, 1), B(EBX, 2), B(EBX, 3),
			W(ECX), W(EDX));
			break;
		case 0x80000002:
		case 0x80000003:
		case 0x80000004:
			printf("%.4s%.4s%.4s%.4s\n", EAX, EBX, ECX, EDX);
			break;
		default:
			printf(
			"cpuid(" H32 "): "
			"eax: " H32 ", "
			"ebx: " H32 ", "
			"ecx: " H32 ", "
			"edx: " H32 "\n",
			cpuid_arg, W(EAX), W(EBX), W(ECX), W(EDX));
		}

	}

	return 0;
}
