#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		char* filename = argv[i];
		int mmapable = 1;

		int fd = open(filename, O_RDONLY);
		if (fd < 0) {
			perror("open");
			return 1;
		}

		void* addr = mmap(0, 4096, PROT_NONE, MAP_PRIVATE, fd, 0);
		if (addr == MAP_FAILED) {
			perror("mmap");
			mmapable = 0;
		}

		printf("%s: %d\n", filename, mmapable);

		close(fd);
	}
}
