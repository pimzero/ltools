#define _GNU_SOURCE
#include <ctype.h>
#include <err.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define ARRSZE(X) (sizeof(X) / sizeof(*(X)))

typedef uint64_t int128[2];

#define PR128(N, S, ...) \
	printf(S ": %016"PRIx64"%016"PRIx64 "\n", __VA_ARGS__, (N)[1],(N)[0])

static struct {
	int e; /* Use fxsave64 */
	int p; /* pattern */
	void (*c)();
	int m, r, x;
} opts;

#define X(N, S) uint64_t N:S;
struct fxsave {
#include "fxsave.inc"
	int128 mm[8];
	int128 xmm[8];
	int128 rsvd4[11];
	int128 avail[3];
} __attribute__((packed));

struct fxsave64 {
#include "fxsave64.inc"
	int128 mm[8];
	int128 xmm[16];
	int128 rsvd4[3];
	int128 avail[3];
} __attribute__((packed));
#undef X


extern void fxsave(struct fxsave*);
extern void fxsave64(struct fxsave64*);
#ifdef __x86_64__
__asm__(
".global fxsave\n"
"fxsave:\n\t"
"fxsave (%rdi)\n\t"
"xor %rax, %rax\n\t"
"ret\n\t"
);

__asm__(
".global fxsave64\n"
"fxsave64:\n\t"
"mov %rdi, %r15\n\t"
".byte 0x48\n\t"
"fxsave (%rdi)\n\t"
"xor %rax, %rax\n\t"
"ret\n\t"
);
#define OPT_64 "e"
#else
__asm__(
".global fxsave\n"
"fxsave:\n\t"
"mov %eax, 8(%esp)\n\t"
"fxsave (%eax)\n\t"
"xor %eax, %eax\n\t"
"ret\n\t"
);
#define OPT_64 ""
#endif

#define def_print(N) \
	static void print_pre_##N(struct N* b); \
	static void print_##N(struct N* b) { \
		print_pre_##N(b); \
		for (size_t i = 0; opts.m && (i < ARRSZE(b->mm)); i++) \
			PR128(b->mm[i], "mm[%zu]", i); \
		for (size_t i = 0; opts.x && (i < ARRSZE(b->xmm)); i++) \
			PR128(b->mm[i], "xmm[%zu]", i); \
		for (size_t i = 0; opts.r && (i < ARRSZE(b->rsvd4)); i++) \
			PR128(b->rsvd4[i], "rsvd[%zu]", i); \
		for (size_t i = 0; opts.r && (i < ARRSZE(b->avail)); i++) \
			PR128(b->avail[i], "avail[%zu]", i); \
	} \
	static void print_pre_##N(struct N* b)

#define X(N, S) printf(#N ": %" PRIx##S "\n", b->N);
def_print(fxsave) {
#include "fxsave.inc"
}

def_print(fxsave64) {
#include "fxsave64.inc"
}
#undef X

static void* mmap_end(size_t sze) {
	char* out = mmap(0, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	if (out == MAP_FAILED)
		err(1, "mmap");

	if (mprotect(out + 4096, 4096, PROT_NONE) < 0)
		err(1, "mprotect");

	return out + 4096 - sze;
}

static void* execute_string(char* s) {
	char* out = mmap(0, strlen(s) + 1, PROT_READ|PROT_WRITE|PROT_EXEC,
			 MAP_PRIVATE|MAP_ANON, -1, 0);
	if (out == MAP_FAILED)
		err(1, "mmap");

	char* cur = out;
	while (*s) {
		while (*s && !isalnum(*s))
			++s;
		if (!*s)
			break;

		if (!isxdigit(*s))
			errx(1, "Invalid hex code '%c'", *s);

		char* nextp = s + 2;
		char next = *nextp;
		*nextp = 0;
		*(cur++) = strtol(s, &s, 16);
		*nextp = next;
	}
	*cur = 0xc3;

	return out;
}

static int help(char* name) {
	fprintf(stderr, "Usage: %s [-mxr"OPT_64"] [-p hex] [-c hex_code]\n"
			"m, x, r: toggle mm, xmm, and reserved fields\n"
			"p: initialization pattern of fxsave area\n"
			"c: x86 code to execute before call to fxsave\n",
		name);
	if (*OPT_64)
		fprintf(stderr, OPT_64": fxsave with rex.w = 1\n");
	return 0;
}

int main(int argc, char** argv) {
	int arg;

	opts.p = 0x5a;

	while ((arg = getopt(argc, argv, OPT_64 "p:c:mxrh")) != -1) {
		switch (arg) {
		case 'e':
			opts.e = 1;
			break;
		case 'p':
			opts.p = strtol(optarg, NULL, 16);
			break;
		case 'c':
			opts.c = execute_string(optarg);
			break;
		case 'm':
			opts.m ^= 1;
			break;
		case 'x':
			opts.x ^= 1;
			break;
		case 'r':
			opts.r ^= 1;
			break;
		case 'h':
			return help(argv[0]);
		}
	}
	if (opts.c)
		opts.c();
#ifdef __x86_64__
	if (opts.e)
	{
		struct fxsave64* buf = mmap_end(sizeof(*buf));
		memset(buf, opts.p, sizeof(*buf));
		fxsave64(buf);
		print_fxsave64(buf);
	} else
#endif
	{
		struct fxsave* buf = mmap_end(sizeof(*buf));
		memset(buf, opts.p, sizeof(*buf));
		fxsave(buf);
		print_fxsave(buf);
	}
}
