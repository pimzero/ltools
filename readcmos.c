#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char** argv) {
	(void) argc;
	int fd = open("/dev/port", O_RDWR);
	if (fd < 0)
		err(1, "open");

	argv++;

	const char* default_args[] = { "0xd", NULL };
	if (!*argv)
		argv = default_args;

	do {
		uint8_t addr = strtol(*argv, NULL, 0);
		addr &= 0x7f;
		if (lseek(fd, 0x70, SEEK_SET) < 0)
			err(1, "lseek(SET)");
		if (write(fd, &addr, 1) < 0)
			err(1, "write");

		if (lseek(fd, 0x71, SEEK_SET) < 0)
			err(1, "lseek(SET)");
		uint8_t val;
		if (read(fd, &val, 1) < 0)
			err(1, "read");

		printf("%#x: %#x\n", addr, val);
		argv++;
	} while (*argv);

	return 0;
}
