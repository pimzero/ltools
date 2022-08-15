#define _GNU_SOURCE

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

#define arrsze(X) (sizeof(X) / sizeof(*(X)))

static int get_spec_ctrl(unsigned long arg) {
	return prctl(PR_GET_SPECULATION_CTRL, arg, 0, 0, 0);
}

static int set_store_bypass(unsigned long arg) {
	return prctl(PR_SET_SPECULATION_CTRL, PR_SPEC_STORE_BYPASS, arg, 0, 0);
}

static const char* vals[] = {
#define X(X) [PR_##X] = #X
	X(SPEC_PRCTL),
	X(SPEC_ENABLE),
	X(SPEC_DISABLE),
	X(SPEC_FORCE_DISABLE),
#undef X
};

static int check_flag(const char* str, const char* flag) {
	if (str[0] == '-' && str[1] == flag[0])
		return 1;

	if (str[0] != '-' || str[1] != '-')
		return 0;

	str = str + 2;
	while (*str && *str == *flag)
		str++, flag++;

	return !*str;
}

static void print_spec_ctrl(void) {
	int val = get_spec_ctrl(PR_SPEC_STORE_BYPASS);
	if (val < 0)
		err(1, "prctl");

	for (size_t i = 1; i < arrsze(vals); i *= 2) {
		if (val & i) {
			const char* str = vals[i];
			if (!str)
				str = "SPEC_???";

			printf("%s\n", str);
		}
	}
}

int main(int argc, char** argv, char** envp) {
	if (argc < 2) {
		print_spec_ctrl();
		return 0;
	}

	if (argc < 3)
		errx(1, "Not enough arguments");

	struct {
		unsigned long val;
		const char* flag;
	} flags[] = {
		{ PR_SPEC_ENABLE, "enable" },
		{ PR_SPEC_DISABLE, "disable" },
		{ PR_SPEC_FORCE_DISABLE, "force-disable" },
	};

	for (size_t i = 0; i < arrsze(flags); i++) {
		if (check_flag(argv[1], flags[i].flag)) {
			if (set_store_bypass(flags[i].val) < 0)
				err(1, "set_store_bypass");

			if (execvpe(argv[2], argv + 2, envp) < 0)
				err(1, "execvpe");
		}
	}

	errx(1, "Invalid arguments");
}
