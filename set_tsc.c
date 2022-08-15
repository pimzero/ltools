#define _GNU_SOURCE

#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <unistd.h>

int main(int argc, char** argv, char** envp) {
	if (argc <= 1) {
		int ret = -1;
		if (prctl(PR_GET_TSC, &ret, 0, 0, 0) < 0)
			err(1, "get_tsc");
		printf("%d\n", ret);
		return 0;
	}

	if (prctl(PR_SET_TSC, PR_TSC_SIGSEGV, 0, 0, 0) < 0)
		err(1, "prctl");

	if (execvpe(argv[1], argv + 1, envp) < 0)
		err(1, "execvpe(\"%s\")", argv[1]);
}
