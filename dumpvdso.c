#include <asm/prctl.h>
#include <err.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <sys/auxv.h>
#include <sys/mman.h>

#define ARRSZE(X) (sizeof(X) / sizeof(*(X)))
#define K 1024
#define M (1024 * K)

struct {
	const char* str;
	int val;
} aliases[] = {
	{ "x32", ARCH_MAP_VDSO_X32 },
	{ "32", ARCH_MAP_VDSO_32 },
	{ "64", ARCH_MAP_VDSO_32 },
};

static int get_kind(const char* str) {
	for (size_t i = 0; i < ARRSZE(aliases); i++) {
		if (!strcmp(aliases[i].str, str))
			return aliases[i].val;
	}
	errx(1, "Alias \"%s\" not found", str);

}

int main(int argc, const char** argv) {
	const char* argv_default[] = {
		argv[0], "64", NULL
	};
	if (argc < 2) {
		argc = ARRSZE(argv_default);
		argv = argv_default;
	}

	unsigned long sysinfo_ehdr = getauxval(AT_SYSINFO_EHDR);
	if (!sysinfo_ehdr)
		err(1, "getauxval(AT_SYSINFO_EHDR)");

	if (munmap(sysinfo_ehdr - 4 * M, 16 * M) < 0)
		err(1, "munmap");


	int kind = get_kind(argv[1]);

	long size = arch_prctl(kind, 0);
	if (size < 0)
		err(1, "ARCH_MAP_VDSO");
}
