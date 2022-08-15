#include <stdio.h>
#include <err.h>
#include <sys/statvfs.h>

int main(int argc, char** argv) {
	struct statvfs st;

	if (argc < 2)
		errx(1, "not enough arguments");

	for (int i = 1; i < argc; i++) {
		if (statvfs(argv[i], &st) < 0)
			err(1, "statvfs");

		printf("%s:\n", argv[i]);
#define X(A, B) printf(" - " #A ": " B "\n", st.f_##A)
		X(bsize, "%zu");
		X(frsize, "%zu");
		X(blocks, "%zu");
		X(bfree, "%zu");
		X(bavail, "%zu");
		printf("\n");
		X(files, "%zu");
		X(ffree, "%zu");
		X(favail, "%zu");
		printf("\n");
		X(fsid, "%zu");
		X(flag, "%zu");
		X(namemax, "%zu");
#undef X
	}
}
