#define _GNU_SOURCE

#include <err.h>
#include <sys/syscall.h>
#include <unistd.h>

static int memfd_secret(unsigned int flags) {
	return syscall(SYS_memfd_secret, flags);
}

int main(int argc, char** argv, char** envp) {
	if (argc < 2)
		errx(1, "Not enough arguments");

	int fd = memfd_secret(0);
	if (fd < 0)
		err(1, "memfd_secret");

	if (execvpe(argv[1], argv + 1, envp))
		err(1, "execvpe");
}
