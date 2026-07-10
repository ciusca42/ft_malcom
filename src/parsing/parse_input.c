#include "../../includes/ft_malcom.h"

int parse_input(char **argv) {
	if (strcmp(argv[1], "192"))
		return 0;
	return 1;
}

int is_valid_flag(char *flag) {

	const char *valid_flags[] = {"--help", "-h", "-v", "--verbose", 0};

	for (int i = 0; valid_flags[i]; i++) {
		if (strcmp(valid_flags[i], flag) != 0) {
			return 0;
		}
	}
	return 1;

}

int check_flags(char **argv) {

	for (int i = 1; argv[i]; i++) {
		if (!is_valid_flag(argv[i]))
			return 0;
	}
	return 1;

	if (strcmp(argv[1], "--help") == 0) {
		print_help();
		return 1;
	}
	return 0;

}