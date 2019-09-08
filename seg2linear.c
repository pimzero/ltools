#include <stdio.h>

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		size_t seg, off;
		sscanf(argv[i], "%lx:%lx", &seg, &off);
		printf("%lx:%lx = %#lx\n", seg, off, 0x10 * seg + off);
	}
}
