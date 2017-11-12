#include <stdint.h>
#include <stdio.h>

/*
 * Usage:
 * sgdt
 * or
 * sgdt (gdt|ldt|idt|tr)+
 */

#define ARRSZE(X) (sizeof(X) / sizeof(*(X)))

struct tr {
	uint16_t limit;
	void* base;
} __packed;

typedef void (*settr)(struct tr*);

static int streq(const char* a, const char* b)
{
	size_t i = 0;

	while (a[i] == b[i] && a[i])
		i++;

	return b[i] == a[i];
}

static void get_gdt(struct tr* tr)
{
	__asm__ __volatile__("sgdt %0\n\t"
			     : : "m" (*tr));
}

static void get_ldt(struct tr* tr)
{
	__asm__ __volatile__("lldt %0\n\t"
			     : : "m" (*tr));
}

static void get_idt(struct tr* tr)
{
	__asm__ __volatile__("lidt %0\n\t"
			     : : "m" (*tr));
}

static void get_tr(struct tr* tr)
{
	__asm__ __volatile__("ltr %0\n\t"
			     : : "m" (*tr));
}

static struct {
	const char* name;
	settr settr;
} funclist[] = {
	{ "gdt", get_gdt },
	{ "ldt", get_ldt },
	{ "idt", get_idt },
	{ "tr", get_tr },
};

static void print_tr(settr settr)
{
	struct tr tr = {0, 0};

	settr(&tr);

	printf("base: %p\nlimit: %x\n", tr.base, tr.limit);
}

int main(int argc, char** argv)
{
	argv++;
	argc--;

	if (argc <= 0) {
		argc = 1;
		argv[0] = "gdt";
	}

	for (int i = 0; i < argc; i++) {
		for (size_t j = 0; j < ARRSZE(funclist); j++) {
			if (!streq(funclist[j].name, argv[i]))
				continue;

			printf("%s:\n", funclist[j].name);
			print_tr(funclist[j].settr);
			break;
		}
	}
}
