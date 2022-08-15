#include <stdio.h>

extern long rdpid(void);

__asm__(
".global rdpid\n"
"rdpid:\n\t"
"rdpid %rax\n\t"
"ret\n\t");

int main(void) {
	printf("%lx\n", rdpid());
}
