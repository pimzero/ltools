#define _POSIX_C_SOURCE 2
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv) {
	int opt;
	int (*f)(void)  = getpid;

	while ((opt =getopt(argc, argv, "ph")) != -1) {
		switch (opt) {
		case 'h':
			break;
		case 'p':
			f = getppid;
		}
	}

	printf("%d\n", f());
}
