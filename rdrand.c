#include <stddef.h>
#include <unistd.h>

#if __x86_64__
#define REG "r"
#else
#define REG "e"
#endif

int main() {
	while (1) {
		size_t tmp;
		__asm__("1:\n\t"
			"rdrand %0\n\t"
			"jnb 1b\n\t"
			: "=d"(tmp));
		write(1, &tmp, sizeof(tmp));
	}
}
