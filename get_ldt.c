#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <asm/ldt.h>
#include <stdio.h>
#include <syscall.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

#define SEGMENTS X(cs) X(ds) X(es) X(fs) X(gs) X(ss)
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

#define LDT_ENTRIES 8192

static int modify_ldt(int func, struct user_desc* ptr, unsigned long count) {
    return syscall(SYS_modify_ldt, func, ptr, count);
}

static void dump_ldt(void) {
	struct user_desc arr[LDT_ENTRIES];
	int ret = modify_ldt(0, arr, sizeof(arr));

#if 1
	for (size_t i = 0; i < ret / sizeof(struct user_desc); i++) {
		printf("[%d] = { %d }\n", i, arr[i].base_addr);
	}
#endif
}

union selector {
	uint16_t n;
	struct {
		uint16_t rpl:2;
		uint16_t ti:1;
		uint16_t index:13;
	};
};

int main(void) {
	dump_ldt();

#include <string.h>

	for (int i = 3; i < 24; i++) {
		struct user_desc entry;
		memset(&entry, -1, sizeof(entry));
		entry.entry_number = i;
		entry.contents = 0;
#if 1
		if (modify_ldt(1, &entry, sizeof(entry)) < 0)
			err(1, "modify_ldt");
#endif
	}

	//GDT_ENTRY_INIT(0xc0f3, 0, 0xfffff),
	struct user_desc entry = {
		.entry_number = 1,
		.base_addr = 2,
		.limit = 0xfffff,
		.seg_32bit = 1,
		.contents = 1,


	};
	if (modify_ldt(1, &entry, sizeof(entry)) < 0)
		err(1, "modify_ldt");

	union selector a = {
		.rpl = 3,
		.ti = 1,
		.index = 1,
	};

	set_ds(a.n);

	//dump_ldt();
}
