CFLAGS=-std=c99 -Wall -Wextra -O2
all:\
	bufpipe \
	coredump_filter \
	cpuid \
	dumpauxv \
	fxsave \
	mmapable \
	rdrand \
	readcmos \
	readio \
	seg2linear \
	sgdt \
	smsw \
	writeio \
	xgetbv

coredump_filter:
