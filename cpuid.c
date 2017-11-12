#include <unistd.h>
#include <stdint.h>

#if __x86_64__
#define REG "r"
#else
#define REG "e"
#endif

static void print_hex(const void* val, size_t size)
{
	static const char chars[] = "0123456789ABCDEF";
	char data[size * 2];
	const char* a = val;

	for (size_t i = 0; i < sizeof(data); i++) {
		data[i * 2] = chars[a[i] & 0xf];
		data[i * 2 + 1] = chars[(a[i] & 0xf0) >> 4];
	}

	write(STDOUT_FILENO, "0x", 2);
	write(STDOUT_FILENO, data, sizeof(data));
}

int main(void)
{
	static union {
		char c[16];
		uint32_t i[4];
	} val;

	__asm__ __volatile__("mov %%" REG "ax, %%" REG "si\n\t"
			     "mov $0, %%" REG "ax\n\t"
			     "cpuid\n\t"
			     "mov %%ebx, (%%" REG "si)\n\t"
			     "mov %%edx, 4(%%" REG "si)\n\t"
			     "mov %%ecx, 8(%%" REG "si)\n\t"
			     "mov %%eax, 12(%%" REG "si)\n\t"
			     : : "a" (&val) : "ebx", "ecx", "edx", REG "si" );

	write(STDOUT_FILENO, val.c, 12);
	write(STDOUT_FILENO, "\n", 1);

	print_hex(&val.i[3], sizeof(*val.i));
	write(STDOUT_FILENO, "\n", 1);

	return 0;
}
