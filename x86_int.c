#define _GNU_SOURCE

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define PROT_RW (PROT_READ|PROT_WRITE)
#define PROT_RX (PROT_READ|PROT_EXEC)

#define ARRSZE(X) (sizeof(X) / sizeof(*(X)))

enum regs {
	R_RAX, R_RCX, R_RDX, R_RBX, R_RSP, R_RBP, R_RSI, R_RDI,
	R_R8 = 0x100, R_R9, R_R10, R_R11, R_R12, R_R13, R_R14, R_R15,
	R_UNDEF = -1
};

static struct {
	enum regs val;
	const char* name;
} regs_args[] = {
#define X(R) { R_##R, #R }
	X(RAX), X(RBX), X(RCX), X(RDX), X(RBP), X(RSI), X(RDI), X(R8), X(R9),
	X(R10), X(R11), X(R12), X(R13), X(R14), X(R15)
#undef X
};

#define B(Ptr, X) *(Ptr)++ = (X) & 0xff
#define INT(X, Ptr) do { B(Ptr, 0xcd); B(Ptr, X); } while (0)
#define RET(Ptr) do { B(Ptr, 0xc3); } while (0)
#define MOV(Val, Reg, Ptr) \
do { \
	B(Ptr, ((0x48b8 + Reg) >> 8)); \
	B(Ptr, ((0x48b8 + Reg) >> 0)); \
	for (int i = 0; i < 8; i++) \
		B(Ptr, ((Val) >> (8 * i))); \
} while (0);

static unsigned long parse_ul_or_err(const char* str) {
	char* success = NULL;
	unsigned long val = strtoul(str, &success, 0);
	if (*success)
		err(1, "strtoul");

	return val;
}

int main(int argc, char** argv) {
	if (argc < 2)
		errx(1, "not enough arguments");

	void* const page = mmap(0, 4096, PROT_RW, MAP_PRIVATE|MAP_ANON, -1, 0);
	if (page == MAP_FAILED)
		err(1, "mmap");

	unsigned char* ptr = page;
	for (int i = 1; i < argc; i++) {
		if (mprotect(page, 4096, PROT_RW) < 0)
			err(1, "protect(RW)");

		char* sep = NULL;
		while (argv[i] && (sep = strchr(argv[i], '='))) {
			*sep = 0;
			enum regs reg = R_UNDEF;
			for (size_t j = 0; j < ARRSZE(regs_args); j++) {
				if (!strcasecmp(argv[i], regs_args[j].name))
					reg = regs_args[j].val;
			}

			if (reg == R_UNDEF)
				errx(1, "unknown register \"%s\"", argv[i]);

			unsigned long imm = parse_ul_or_err(sep + 1);

			printf("%s=%#lx\n", argv[i], imm);

			MOV(imm, reg, ptr);

			i++;
		}

		unsigned char nr = parse_ul_or_err(argv[i]);

		printf("int $%#x\n", nr);

		INT(nr, ptr);
		RET(ptr);

		if (mprotect(page, 4096, PROT_RX) < 0)
			err(1, "protect(RX)");

		fflush(NULL);

		((void (*)())page)();
	}
}
