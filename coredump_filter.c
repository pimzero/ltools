#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define arrsze(X) (sizeof(X) / sizeof(*(X)))

extern char* optarg;
static char* progname;

static void help(void) {
	fprintf(stderr, "Usage: %s pid [-s val]\n", progname);
}

static void fail(const char* str) {
	perror(str);
	exit(1);
}

static const char* bits[] = {
	"ANON_PRIVATE",
	"ANON_SHARED",
	"MAPPED_PRIVATE",
	"MAPPED_SHARED",
	"ELF_HEADERS",
	"HUGETLB_PRIVATE",
	"HUGETLB_SHARED",
	"DAX_PRIVATE",
	"DAX_SHARED",
};

static int print_bits(int fd, unsigned arg) {
	(void)arg;

	char buf[512];
	if (read(fd, buf, sizeof(buf)) < 0)
		fail("read");

	unsigned val = strtoul(buf, NULL, 16);

	for (size_t i = 0; i < arrsze(bits); i++) {
		if (val & (1 << i)) {
			printf("%s", bits[i]);
			val ^= (1 << i);
			putchar(val ? '|' : '\n');
		}
	}

	return 0;
}

static int set_bits(int fd, unsigned arg) {
	char buf[512];
	int sz = snprintf(buf, sizeof(buf), "%x\n", arg);
	if (write(fd, buf, sz) < 0)
		fail("write");

	return 0;
}

static unsigned str2bit(const char* str) {
	for (size_t i = 0; i < arrsze(bits); i++) {
		if (!strcmp(bits[i], str))
			return 1 << i;
	}

	fprintf(stderr, "Unknown bit \"%s\"\n", str);
	exit(1);
}

static unsigned str2bits(char* str) {
	unsigned out = 0;
	char* ptr = NULL;
	out = strtoul(str, &ptr, 0);

	if (!*ptr)
		return out;

	out = 0;
	char* tok;
	while ((tok = strtok(str, ",| "))) {
		str = NULL;
		out |= str2bit(tok);
	}

	return out;
}

static int open_coredump_filter(const char* arg, int flags) {
	if (!arg || !strcmp(arg, "-"))
		return (flags == O_RDONLY) ? STDIN_FILENO : STDOUT_FILENO;

	char fname[512];

	int pid = atoi(arg);
	snprintf(fname, sizeof(fname), "/proc/%d/coredump_filter", pid);

	int fd = open(fname, flags);
	if (fd < 0)
		fail("open");

	return fd;
}

int main(int argc, char** argv) {
	int opt;
	progname = argv[0];

	int open_flags = O_RDONLY;
	int (*action)(int, unsigned) = print_bits;
	unsigned arg = 0;

	while ((opt = getopt(argc, argv, "s:")) != -1) {
		switch (opt) {
		case 's':
			action = set_bits;
			arg = str2bits(optarg);
			open_flags = O_WRONLY;
			break;
		default:
			help();
			return 1;
		}
	}

	return action(open_coredump_filter(argv[optind], open_flags), arg);
}
