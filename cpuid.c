#include <unistd.h>

int main() {
	static char name[13];
	char* address = name;
	__asm__ __volatile__("mov $0, %%eax\n\t"
			     "cpuid\n\t"
			     "mov %0, %%eax\n\t"
			     "mov %%ebx, (%%eax)\n\t"
			     "mov %%edx, 4(%%eax)\n\t"
			     "mov %%ecx, 8(%%eax)\n\t"
			     : : "m" (address));

	name[sizeof(name) - 1] = '\n';
	write(STDOUT_FILENO, name, sizeof(name));
}
