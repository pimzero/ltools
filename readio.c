#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
	(void) argc;
	int fd = open("/dev/port", O_RDWR);
	if (fd < 0)
		err(1, "open");

	argv++;

	while (*argv) {
		uint32_t addr = strtol(*argv, NULL, 0);
		if (lseek(fd, addr, SEEK_SET) < 0)
			err(1, "lseek(SET)");
		uint8_t val;
		if (read(fd, &val, 1) < 0)
			err(1, "read");

		printf("%#x: %#x\n", addr, val);
		argv++;
	}

	return 0;
}
