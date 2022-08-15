#include <stdint.h>
#include <stdio.h>

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		unsigned segm, offset;
		sscanf(argv[i], "%x:%x", &segm, &offset);
		printf("%x:%x: %#x\n", segm, offset,
			segm * 0x10 + offset);

	}
	return 0;
}
