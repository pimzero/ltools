#define _GNU_SOURCE

#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

int main(int argc, char** argv, char** envp) {
	unsigned long pid = PR_SET_PTRACER_ANY;

	if ((argc > 2) && !memcmp(argv[1], "-p", 2)) {
		pid = atoi(argv[1] + 2);
		argc--;
		argv++;
	}

	if ((argc > 2) && !strcmp(argv[1], "--")) {
		argc--;
		argv++;
	}

	if (argc < 2)
		errx(1, "Not enough arguments");

	if (prctl(PR_SET_PTRACER, pid) < 0)
		err(1, "prctl");

	if (execvpe(argv[1], argv + 1, envp) < 0)
		err(1, "execvpe(\"%s\")", argv[1]);
}
