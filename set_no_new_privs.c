#define _GNU_SOURCE

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <unistd.h>

int main(int argc, char** argv, char** envp) {

	if (argc <= 1) {
		printf("%d\n", prctl(PR_GET_NO_NEW_PRIVS, 0, 0, 0, 0));
		return 0;
	}

	if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0)
		err(1, "prctl");

	if (execvpe(argv[1], argv + 1, envp) < 0)
		err(1, "execvpe(\"%s\")", argv[1]);
}
