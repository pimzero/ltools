#include <err.h>
#include <fcntl.h>
#include <linux/hpet.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void print_info(int fd) {
	struct hpet_info hpet_info;

	if (ioctl(fd, HPET_INFO, &hpet_info) < 0)
		err(1, "ioctl(HPET_INFO)");

#define X(Field, Fmt, Struct) \
	do { printf(#Field ": %" Fmt "\n", Struct.Field); } while(0)
	X(hi_ireqfreq, "ld", hpet_info);
	X(hi_flags, "lx", hpet_info);
	X(hi_hpet, "d", hpet_info);
	X(hi_timer, "d", hpet_info);
}

int main(int argc, const char** argv) {
	(void) argc;
	(void) argv;

	const char* default_argv[] = {
		argv[0],
		"/dev/hpet",
		NULL
	};

	if (argc < 2)
		argv = default_argv;

	int fd = open("/dev/hpet", O_RDONLY);
	if (fd < 0)
		err(1, "open");


	print_info(fd);
	if (ioctl(fd, HPET_IRQFREQ, 1000) < 0)
		err(1, "ioctl(HPET_IRQFREQ)");
	print_info(fd);

#if 0
	unsigned long buf = 0;

	if (read(fd, &buf, 200) < 0)
		err(1, "read");

	printf("%lx\n", buf);
#endif
}
