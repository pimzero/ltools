#include <stdint.h>
#include <stdio.h>

struct gdtr {
	uint16_t limit;
	uint32_t base;
} __packed;

int main() {
	struct gdtr gdtr;
	__asm__ __volatile__("sgdt %0\n\t"
			     : : "m" (gdtr));

	printf("base: %p\nlimit: %p\n", gdtr.base, gdtr.limit);

}
