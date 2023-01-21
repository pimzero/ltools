#define _GNU_SOURCE

#include <err.h>
#include <inttypes.h>
#include <linux/futex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <unistd.h>

#define ARRSZE(X) (sizeof(X) / sizeof(*(X)))

static long get_robust_list(int pid,
			    struct robust_list_head **head_ptr,
			    size_t* len_ptr) {
	return syscall(SYS_get_robust_list, pid, head_ptr, len_ptr);
}

static void remote_iter_robust_list(pid_t pid,
			       struct robust_list_head* remote_head_ptr,
			       struct robust_list_head* head_ptr,
			       void (*f)(void*, struct robust_list, uint32_t)) {
	long offset = head_ptr->futex_offset;
	struct robust_list* remote_ptr = head_ptr->list.next;


	while (remote_ptr) {
		struct robust_list current;
		uint32_t futex_word;
		struct iovec local[2] = {
			{
				.iov_base = &current,
				.iov_len = sizeof(current),
			},
			{
				.iov_base = &futex_word,
				.iov_len = sizeof(futex_word),
			}
		};
		struct iovec remote[2] = {
			{
				.iov_base = remote_ptr,
				.iov_len = sizeof(current),
			},
			{
				.iov_base = remote_ptr + offset,
				.iov_len = sizeof(futex_word),
			}
		};
		if (process_vm_readv(pid, local, ARRSZE(local), remote,
				     ARRSZE(remote), 0) < 0) {
			warn("process_vm_readv");
			return;
		}

		f(remote_ptr, current, futex_word);

		remote_ptr = current.next;
		if (remote_ptr == &(remote_head_ptr->list))
			return;
	}
}

static void print_futex_word(void* list_ptr,
			     struct robust_list list,
			     uint32_t futex_word) {
	printf("-> %p: { word: %" PRIu32 ", next: %p }",
	       list_ptr, futex_word, list.next);
}

static void handle_robust_list_head(pid_t pid,
				    struct robust_list_head* head_ptr) {
	struct robust_list_head head;

	struct iovec local[1] = {
		{
			.iov_base = &head,
			.iov_len = sizeof(head),
		}
	};
	struct iovec remote[1] = {
		{
			.iov_base = head_ptr,
			.iov_len = sizeof(head),
		}
	};

	if (process_vm_readv(pid, local, ARRSZE(local), remote,
			     ARRSZE(remote), 0) < 0) {
		warn("process_vm_readv");
		return;
	}

	remote_iter_robust_list(pid, head_ptr, &head, print_futex_word);
}

int main(int argc, char** argv) {
	if (argc < 2)
		errx(1, "Not enough arguments");

	for (int arg = 1; arg < argc; arg++) {
		char* endptr = NULL;
		long pid = strtol(argv[arg], &endptr, 0);

		if (*endptr) {
			warnx("Invalid pid \"%s\"", argv[arg]);
			continue;
		}

		size_t len_ptr = 0;
		struct robust_list_head* head_ptr;

		if (get_robust_list(pid, &head_ptr, &len_ptr))
			warn("get_robust_list");

		printf("%ld: ", pid);
		if (head_ptr)
			handle_robust_list_head(pid, head_ptr);
		printf("\n");
	}

	return 0;
}
