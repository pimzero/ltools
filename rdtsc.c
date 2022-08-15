#include <inttypes.h>
#include <stdio.h>

static uint64_t rdtsc() {
	uint32_t eax = 0, edx = 0;

	__asm__ volatile(".byte 0x66\n\trdtsc" : "=&a"(eax), "=&d"(edx));
	//__asm__ volatile("rdtsc" : "=&a"(eax), "=&d"(edx));

	return eax | ((uint64_t)edx << 32);
}

int main(void) {
	uint64_t ret = rdtsc();

	printf("%" PRIx64 "\n", ret);
}
