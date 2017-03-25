#include <unistd.h>
#include <stdint.h>

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
	void* address = &val;

	__asm__ __volatile__("mov %%eax, %%esi\n\t"
			     "mov $0, %%eax\n\t"
			     "cpuid\n\t"
			     "mov %%ebx, (%%esi)\n\t"
			     "mov %%edx, 4(%%esi)\n\t"
			     "mov %%ecx, 8(%%esi)\n\t"
			     "mov %%eax, 12(%%esi)\n\t"
			     : : "a" (&val));

	write(STDOUT_FILENO, val.c, 12);
	write(STDOUT_FILENO, "\n", 1);

	print_hex(&val.i[3], sizeof(*val.i));
	write(STDOUT_FILENO, "\n", 1);

	return 0;
}
