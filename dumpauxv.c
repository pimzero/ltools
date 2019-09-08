#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef unsigned long ulong;

int wrap_open(const char* str) {
	if (!strcmp(str, "-"))
		return 0;

	int fd = open(str, O_RDONLY);
	if (fd < 0)
		perror(str);

	return fd;
}

int pr_u(ulong x) {
	return printf("%lu", x);
}

int pr_x(ulong x) {
	return printf("%#lx", x);
}

struct {
	ulong type;
	const char* str;
	int (*pr)(ulong);
} parser[] = {
#define X(X, print) { .type = AT_##X, .str = #X, .pr = pr_##print }
	X(BASE, x),
	X(BASE_PLATFORM, x),
	X(IGNORE, x),
	X(EXECFD, x),
	X(PHDR, x),
	X(PHENT, u),
	X(PHNUM, u),
	X(PAGESZ, u),
	X(BASE, x),
	X(FLAGS, x),
	X(ENTRY, x),
	X(NOTELF, x),
	X(UID, u),
	X(EUID, u),
	X(GID, u),
	X(EGID, u),
	X(CLKTCK, u),
	X(PLATFORM, x),
	X(HWCAP, x),
	X(FPUCW, x),
	X(DCACHEBSIZE, x),
	X(ICACHEBSIZE, x),
	X(UCACHEBSIZE, x),
	X(IGNOREPPC, x),
	X(SECURE, x),
	X(BASE_PLATFORM, x),
	X(RANDOM, x),
	X(HWCAP2, x),
	X(EXECFN, x),
	X(SYSINFO, x),
	X(SYSINFO_EHDR, x),
	X(L1I_CACHESHAPE, x),
	X(L1D_CACHESHAPE, x),
	X(L2_CACHESHAPE, x),
	X(L3_CACHESHAPE, x),
	X(L1I_CACHESIZE, x),
	X(L1I_CACHEGEOMETRY, x),
	X(L1D_CACHESIZE, x),
	X(L1D_CACHEGEOMETRY, x),
	X(L2_CACHESIZE, x),
	X(L2_CACHEGEOMETRY, x),
	X(L3_CACHESIZE, x),
	X(L3_CACHEGEOMETRY, x),
#undef X
};

#define arrsze(X) (sizeof(X) / sizeof(*(X)))

static int dump_auxv(int fd) {
	struct { ulong type; ulong val; } at;

	int err;
	while ((err = read(fd, &at, sizeof(at))) > 0 && at.type != AT_NULL) {
		for (size_t j = 0; j < arrsze(parser); j++) {
			if (parser[j].type == at.type) {
				printf("%s:\t", parser[j].str);
				parser[j].pr(at.val);
				printf("\n");
				goto iter;
			}
		}
		printf("%#016lx:\t%#016lx\n", at.type, at.val);
iter:
		;
	}
	if (err < 0)
		perror("read");

	return err;
}

int main(int argc, char** argv) {
	char *argv_no_args[] = {
		"-",
		NULL
	};

	argc--; argv++;

	if (!*argv) {
		argv = argv_no_args;
		argc++;
	}

	for (int i = 0; i < argc; i++) {
		int fd = wrap_open(argv[i]);
		if (fd < 0)
			return 1;

		if (dump_auxv(fd) < 0)
			return 1;
	}

	return 0;
}
