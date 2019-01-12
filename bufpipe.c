#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

static void fail(const char* str) {
	perror(str);
	exit(1);
}

int main(int argc, char** argv, char** envp) {
	(void) argc; (void) argv;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s command line\n", argv[0]);
		return 1;
	}

	int fd = memfd_create("stdin", 0);
	if (fd < 0)
		fail("memfd_create");

	size_t sz = 0;
	ssize_t rd = 0;
	char buf[512];
	while ((rd = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
		sz += rd;

		if (ftruncate(fd, sz) < 0)
			fail("ftruncate");

		if (write(fd, buf, rd) < 0)
			fail("write");
	}
	if (rd < 0)
		fail("read");

	if (lseek(fd, 0, SEEK_SET) < 0)
		fail("lseek");

	if (dup2(fd, STDIN_FILENO) < 0)
		fail("dup2");

	if (close(fd) < 0)
		fail("close");

	if (execvpe(argv[1], argv + 1, envp))
		fail("execvpe");
}
