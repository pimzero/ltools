CFLAGS=-std=c99 -Wall -Wextra -O2

all: \
	bufpipe \
	coredump_filter \
	cpuid \
	dumpauxv \
	mmapable \
	rdrand \
	readcmos \
	readio \
	seg2linear \
	sgdt \
	smsw \
	writeio \
	xgetbv

coredump_filter: CPPFLAGS=-D_POSIX_C_SOURCE=2
coredump_filter:
