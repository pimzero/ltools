CFLAGS=-std=c99 -Wall -Wextra -Wwrite-strings -ggdb
OUT= \
	bufpipe \
	close_range \
	coredump_filter \
	cpuid \
	dumpauxv \
	dumpvdso \
	fxsave \
	get_ldt \
	get_ss \
	getpid \
	getpid32 \
	hpet \
	lsholes \
	mem_cat \
	memfd_secret \
	mmapable \
	parse_segm \
	rdpid \
	rdrand \
	rdtsc \
	readcmos \
	readio \
	seg2linear \
	set_no_new_privs \
	set_ptracer \
	sgdt \
	smsw \
	speculation \
	statvfs \
	udpsend \
	vmcall \
	writeio \
	x86_int \
	xgetbv \

all: $(OUT)

bufpipe: CPPFLAGS=-D_GNU_SOURCE

coredump_filter: CPPFLAGS=-D_POSIX_C_SOURCE=2

getpid32: CFLAGS+=-m32

clean:
	$(RM) $(OUT)

.PHONY: clean
