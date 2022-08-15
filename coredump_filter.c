#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARRSZE(X) (sizeof(X) / sizeof(*(X)))

extern char* optarg;
static char* progname;

static void help(void) {
	fprintf(stderr, "Usage: %s [-s val] -p pid\n"
			"       %s -s val -- command\n",
		progname, progname);

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
		err(1, "read");

	unsigned long val = strtoul(buf, NULL, 16);

	for (size_t i = 0; i < 8 * sizeof(val); i++) {
		if (val & (1ul << i)) {
			if (i < ARRSZE(bits))
				printf("%s", bits[i]);
			else
				printf("1 << %lu", i);
			putchar((2ul << i) < val ? '|' : '\n');
		}
	}

	return 0;
}

static int set_bits(int fd, unsigned arg) {
	char buf[512];
	int sz = snprintf(buf, sizeof(buf), "%u\n", arg);
	if (write(fd, buf, sz) < 0)
		err(1, "write");

	return 0;
}

static unsigned str2bit(const char* str) {
	for (size_t i = 0; i < ARRSZE(bits); i++) {
		if (!strcmp(bits[i], str))
			return 1 << i;
	}

	errx(1, "Unknown bit \"%s\"\n", str);
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

static int open_coredump_filter_pid(pid_t pid, int flags) {
	char fname[512];
	snprintf(fname, sizeof(fname), "/proc/%d/coredump_filter", pid);

	int fd = open(fname, flags);
	if (fd < 0)
		err(1, "open");

	return fd;
}

static int open_coredump_filter(const char* arg, int flags) {
	if (!arg || !strcmp(arg, "-"))
		return (flags == O_RDONLY) ? STDIN_FILENO : STDOUT_FILENO;

	return open_coredump_filter_pid(atoi(arg), flags);
}

static int open_flags = O_RDONLY;
static int (*action)(int, unsigned) = print_bits;
static unsigned arg = 0;

static void reset_opts(void) {
	open_flags = O_RDONLY;
	action = print_bits;
	arg = 0;
}

int main(int argc, char** argv) {
	int opt;
	progname = argv[0];

	while ((opt = getopt(argc, argv, "s:p:")) != -1) {
		switch (opt) {
		case 's':
			action = set_bits;
			arg = str2bits(optarg);
			open_flags = O_WRONLY;
			break;
		case 'p':
			action(open_coredump_filter(optarg, open_flags), arg);
			reset_opts();
			break;
		default:
			help();
		}
	}

	if (argv[optind] && (action != print_bits)) {
		action(open_coredump_filter_pid(getpid(), open_flags), arg);

		if (execvp(argv[optind], argv + optind) < 0)
			err(1, "execvp");

	} else if (argv[optind] || optind < 2 || (action != print_bits))
		help();
}
