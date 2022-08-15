#define _POSIX_C_SOURCE 200112L
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <err.h>
#include <string.h>
#include <stdio.h>

static int dontfrag(int fd) {
	int val = IP_PMTUDISC_DO;
	return setsockopt(fd, IPPROTO_IP, IP_MTU_DISCOVER, &val, sizeof(val));
}

int main(int argc, char** argv) {

	struct addrinfo* ai = NULL;

	if (argc < 3)
		errx(1, "Usage: %s address port\n", *argv);

	int e;

	if ((e = getaddrinfo(argv[1], argv[2], NULL, &ai)) != 0)
		errx(1, "%s", gai_strerror(e));

	for (struct addrinfo* itr = ai; itr; itr = itr->ai_next) {
		if (itr->ai_addr->sa_family == AF_INET) {
			struct sockaddr_in* addr = (void*)itr->ai_addr;
			union {
				uint32_t u32;
				uint8_t u8[4];
			} ip = { .u32 = addr->sin_addr.s_addr };

			printf("%hhu.%hhu.%hhu.%hhu\n",
				ip.u8[0], ip.u8[1], ip.u8[2], ip.u8[3]);

			int sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (sock < 0)
				err(1, "socket");

			if (dontfrag(sock) < 0)
				err(1, "dontfrag");

			char (*buf)[65535] = malloc(sizeof(*buf));
			if (!buf)
				err(1, "malloc");

			ssize_t bufsz = read(0, buf, sizeof(*buf));
			if (bufsz < 0)
				err(1, "read");

			if (sendto(sock, buf, bufsz, 0, (struct sockaddr*)addr,
				   sizeof(*addr)) < 0)
				err(1, "sendto");


			exit(0);

		} else
			printf("???\n");
	}

}
