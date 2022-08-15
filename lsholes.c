#define _GNU_SOURCE
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define ARRSZE(X) (sizeof(X) / sizeof(*(X)))

struct verbose_data {
	off_t prev;
	const char* fmt;
};

static void* verbose_init(void) {
	struct verbose_data* ret = calloc(1, sizeof(struct verbose_data));
	ret->fmt = "%s: %ld-%ld\n";

	return ret;
};

static void* hex_init(void) {
	struct verbose_data* ret = calloc(1, sizeof(struct verbose_data));
	ret->fmt = "%s: 0x%016lx-0x%016lx\n";

	return ret;
};

static void verbose_teardown(void* data) {
	free(data);
};

static void verbose_loop(void* data, int whence, off_t offset) {
	struct verbose_data* d = data;

	if (offset == d->prev)
		return;

	const char* whence_str;
	if (whence == SEEK_HOLE)
		whence_str = "data";
	else if (whence == SEEK_DATA)
		whence_str = "hole";
	else
		errx(1, "unsupported \"whence\"");

	printf(d->fmt, whence_str, d->prev, offset);

	d->prev = offset;
}

struct count_data {
	size_t count_holes, count_data;
	off_t prev;
};

static void* count_init(void) {
	return calloc(1, sizeof(struct count_data));
};

static void count_teardown(void* data) {
	struct count_data* d = data;

	printf("hole: %zu\n", d->count_holes);
	printf("data: %zu\n", d->count_data);
	free(data);
};

static void count_loop(void* data, int whence, off_t offset) {
	struct count_data* d = data;

	size_t *count_ptr = NULL;
	if (whence == SEEK_HOLE)
		count_ptr = &(d->count_holes);
	else if (whence == SEEK_DATA)
		count_ptr = &(d->count_data);
	else
		errx(1, "unsupported \"whence\"");
	*count_ptr += offset - d->prev;
	d->prev = offset;
}

struct {
	void* (*init)(void);
	void (*teardown)(void*);
	void (*loop)(void*, int, off_t);
} modes[] = {
	[0] = { verbose_init, verbose_teardown, verbose_loop },
	['c'] = { count_init, count_teardown, count_loop },
	['x'] = { hex_init, verbose_teardown, verbose_loop },
};;

static int do_file(const char* path, unsigned int mode) {
	if (mode >= ARRSZE(modes) || !modes[mode].loop)
		errx(1, "unsupported mode");

	int fd = open(path, O_RDONLY);
	if (fd < 0)
		err(1, "open");

	struct stat st;
	if (fstat(fd, &st) < 0)
		err(1, "fstat");

	const int whences[2] = { SEEK_DATA, SEEK_HOLE };

	void* data = modes[mode].init();

	off_t cur = 0;
	while (cur < st.st_size) {
		for (size_t i = 0; i < ARRSZE(whences); i++) {
			cur = lseek(fd, cur, whences[i]);
			if (cur < 0) {
				if (errno != ENXIO)
					err(1, "lseek");
				cur = st.st_size;
			}
			modes[mode].loop(data, whences[i], cur);
		}
	}

	modes[mode].teardown(data);

	if (close(fd) < 0)
		err(1, "close");

	return 0;
}

int main(int argc, char** argv) {
	if (argc < 2)
		errx(1, "Not enough arguments");

	int opt;
	unsigned int mode = 0;

	while ((opt = getopt(argc, argv, "cx")) != -1) {
		switch (opt) {
		case 'c':
		case 'x':
			mode = opt;
			break;
		}
	}

	for (int i = optind; i < argc; i++)
		do_file(argv[i], mode);
}
