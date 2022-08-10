CFLAGS=-std=c99 -Wall -Wextra -Wwrite-strings -ggdb
OUT= \
	bufpipe \
	coredump_filter \
	cpuid \
	dumpauxv \
	fxsave \
	lsholes \
	mmapable \
	rdrand \
	readcmos \
	readio \
	seg2linear \
	sgdt \
	smsw \
	writeio \
	xgetbv \

all: $(OUT)

clean:
	$(RM) $(OUT)

.PHONY: clean
