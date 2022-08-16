CFLAGS=-std=c99 -Wall -Wextra -Wwrite-strings -ggdb
OUT= \
	bufpipe \
	close_range \
	coredump_filter \
	cpuid \
	dumpauxv \
	fxsave \
	getpid \
	getpid32 \
	get_ss \
	lsholes \
	memfd_secret \
	mmapable \
	parse_segm \
	rdpid \
	rdrand \
	rdtsc \
	readcmos \
	readio \
	seg2linear \
	setcpuid \
	set_no_new_privs \
	set_ptracer \
	set_tsc \
	sgdt \
	smsw \
	speculation \
	statvfs \
	udpsend \
	vmcall \
	writeio \
	x86_int \
	xgetbv \

OUT_WIP = \
	dumpvdso \
	get_ldt \
	hpet \
	mem_cat \

all: $(OUT)

all-wip: $(OUT) $(OUT_WIP)

bufpipe: CPPFLAGS=-D_GNU_SOURCE

coredump_filter: CPPFLAGS=-D_POSIX_C_SOURCE=2

getpid32: CFLAGS+=-m32

clean:
	$(RM) $(OUT) $(OUT_WIP)

.PHONY: clean all
