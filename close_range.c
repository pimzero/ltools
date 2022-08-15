#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int close_range(int fd, int max_fd) {
#ifdef __NR_close_range
	int err = syscall(__NR_close_range, fd, max_fd, 0);
	if (err >= 0)
		return err;
#endif
	for (int i = fd; i < max_fd; i++)
		close(i);

	return 0;
}

static void parse_arg(char* str, int* start, int* end) {
	char* endptr = NULL;

	*start = strtol(str, &endptr, 10);
	*end = *start;

	if (*endptr == 0)
		return;

	if (*endptr != ':' || !endptr[1])
		errx(1, "invalid argument \"%s\"", str);

	*end = strtol(endptr + 1, &endptr, 10);
}


int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		if (!strcmp("--", argv[i])) {
			if (execvp(argv[i + 1], argv + i + 1) < 0)
				err(1, "execvp");
		}
		int start, end;
		parse_arg(argv[i], &start, &end);
		if (start == end)
			close(start);
		else
			close_range(start, end);

	}

	errx(1, "No command given");
}
