#include <stdio.h>
#include <unistd.h>

int main() {
	printf("%u\n", getpid());

	while (1)
		;
}
