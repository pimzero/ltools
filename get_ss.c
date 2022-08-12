#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define SEGMENTS X(cs) X(ds) X(es) X(fs) X(gs) X(ss)

#define ARRSZE(X) (sizeof(X) / sizeof(*(X)))

union selector {
	uint16_t n;
	struct {
		uint16_t rpl:2;
		uint16_t ti:1;
		uint16_t index:13;
	};
};

#define GET(X) uint16_t get_##X(void); \
		__asm__(".global get_" #X "\n" \
			"get_" #X ":\n\t" \
			"movw %" #X ", %ax\n\t" \
			"ret\n")

#define SET(X) void set_##X(uint16_t eax) { \
		__asm__ volatile("mov %[ax], %%" #X "\n" : : [ax] "r"(eax)); \
	}

#define X(S) GET(S); SET(S);
SEGMENTS
#undef X

const char* all[] = {
#define X(S) #S,
	SEGMENTS
#undef X
	NULL
};

static const struct {
	const char* s;
	uint16_t (*f)(void);
} regs[] = {
#define X(S) { #S, get_##S },
	SEGMENTS
#undef X
};

int main(int argc, const char** argv) {
	const char** to_read = all;
	if (argc > 1)
		to_read = argv + 1;

	set_ds(43);

	for (size_t i = 0; to_read[i]; i++) {
		for (size_t j = 0; j < ARRSZE(regs); j++) {
			if (strcmp(to_read[i], regs[j].s))
				continue;

			union selector s = { .n = regs[j].f() };
			printf("%s: %u (index: %u, ti: %u, rpl: %u)\n",
			       regs[j].s, s.n, s.index, s.ti, s.rpl);
			break;
		}
	}

}
