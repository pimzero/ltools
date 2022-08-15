#define _POSIX_C_SOURCE 200809L
#include <err.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef unsigned long ulong;

#define PGSZ 4096

static const char* perms_to_read = "rw-p";

struct map_entry {
	ulong start, end;
	int perms[4];
	size_t offset;
};

struct dumper_data {
	const int fd;
};

static int maps_iterate(long pid, int (*f)(struct map_entry*, void*),
			void* data) {
	(void) f;
	char fname[128];

	snprintf(fname, sizeof(fname) - 1, "/proc/%ld/maps", pid);
	FILE* file = fopen(fname, "r");
	if (!file)
		err(1, "open");

	char* line = NULL;
	size_t len = 0;
	ssize_t nread;
	struct map_entry entry;

	while ((nread = getline(&line, &len, file)) != -1) {
		char perms[5];
		int dev_major, dev_minor;
		sscanf(line, "%lx-%lx %4c %lx %x:%x",
		       &entry.start, &entry.end, perms, &entry.offset,
		       &dev_major, &dev_minor);
		memcpy(entry.perms, perms, sizeof(entry.perms));

		if (!memcmp(entry.perms, perms_to_read, 4))
			f(&entry, data);
	}

	free(line);
	fclose(file);

	return 0;
}

static int dump_mem(ulong start, ulong end, struct dumper_data* data) {
	int fd = data->fd;

	if (lseek(fd, start, SEEK_SET) < 0)
		err(1, "lseek");

	for (size_t toread = end - start; toread; toread -= PGSZ) {
		char buf[PGSZ];
		if (read(fd, buf, sizeof(buf)) != PGSZ)
			err(1, "read");

		if (write(STDOUT_FILENO, buf, sizeof(buf)) < 0)
			err(1, "write");

	}
#if 0
	if (sendfile(STDOUT_FILENO, fd, NULL, 4096) < 0)
		err(1, "sendfile");
#endif

	return 0;
}

static int dump_mem_wrapper(struct map_entry* map_entry, void* data) {
	struct dumper_data* dumper_data = data;
	ulong start = map_entry->start, end = map_entry->end;


	return dump_mem(start, end, dumper_data);
}

int main(int argc, char** argv) {
	if (argc <= 1)
		errx(1, "Not enough arguments");

	long pid = strtol(argv[1], NULL, 10);
	if (pid <= 0 || pid >= LONG_MAX)
		err(1, "strtol");

	char fname[128];
	snprintf(fname, sizeof(fname) - 1, "/proc/%ld/mem", pid);
	struct dumper_data dumper_data = { .fd = open(fname, O_RDONLY) };
	if (dumper_data.fd < 0)
		err(1, "open");

	maps_iterate(pid, dump_mem_wrapper, &dumper_data);
}
