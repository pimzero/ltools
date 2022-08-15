#define _GNU_SOURCE

#include <asm/prctl.h>
#include <err.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>

int arch_prctl(int code, unsigned long addr);

int main(int argc, char** argv, char** envp) {
	if (argc <= 1) {
		int cpuid_enabled = arch_prctl(ARCH_GET_CPUID, 0);
		if (cpuid_enabled < 0)
			err(1, "ARCH_GET_CPUID");
		printf("%d\n", cpuid_enabled);

		return 0;
	}

	if (arch_prctl(ARCH_SET_CPUID, atoi(argv[1])) < 0)
		err(1, "ARCH_SET_CPUID");

	if (execvpe(argv[2], argv + 2, envp) < 0)
		err(1, "execvpe");
}
