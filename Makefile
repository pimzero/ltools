CFLAGS=-Wall -Wextra -std=c99 -O2

all: \
	bufpipe \
	cpuid \
	coredump_filter \
	dumpauxv \
	sgdt \
	smsw \
	xgetbv

coredump_filter: CPPFLAGS=-D_POSIX_C_SOURCE=2
coredump_filter:
