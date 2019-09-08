#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char** argv) {
	(void) argc;
	if (argc < 3)
		errx(1, "missing arguments. Usage: %s port value", argv[0]);

	int fd = open("/dev/port", O_RDWR);
	if (fd < 0)
		err(1, "open");

	uint32_t addr = strtol(argv[1], NULL, 0);
	uint8_t val = strtol(argv[2], NULL, 0);

	if (lseek(fd, addr, SEEK_SET) < 0)
		err(1, "lseek(SET)");
	if (write(fd, &val, 1) < 0)
		err(1, "write");

	return 0;
}
