#include "../../includes/ft_malcom.h"

int parse_address(const char **argv, t_args *args) {

    if (!try_parse(argv, '.', parse_ip, args->source_ip) && !resolve_hostname((char*)*argv, args->source_ip)) {
        parsing_error("Unknown host or invalid ip address -> ", (char*)*argv);
        return 0;
    }
    argv++;
    if (!try_parse(argv, ':', parse_mac, args->source_mac)) {
        parsing_error("invalid mac address -> ", (char*)*argv);
        return 0;
    }

    argv++;
    if (!try_parse(argv, '.', parse_ip, args->target_ip) && !resolve_hostname((char*)*argv, args->source_ip)) {
        parsing_error("Unknown host or invalid ip address -> ", (char*)*argv);
        return 0;
    }
    argv++;
    if (!try_parse(argv, ':', parse_mac, args->target_mac)) {
         parsing_error("invalid mac address -> ", (char*)*argv);
        return 0;
    }
    argv++;
    if (*argv != 0) {
        parsing_error("Invalid argument -> ", (char*)*argv);
        usage();
        return 0;
    }
    return 1;
}

int parse_input(const char **argv, const int argc, t_args *args) {

	int flag_ret;

	args->verbose = 0;

	if (argc < 2) {
		fprintf(stderr, "\nInvalid Arguments\n");
		usage();
		return 0;
	}

	if (argc == 2 && !ft_strncmp(argv[1], "help", ft_strlen(argv[1]))) {
        print_help();
        exit (0);
	}

	if (argc < 5) {
		fprintf(stderr, "\nInvalid Arguments\n");
		usage();
		return 0;
	}

	// skip argv[0] -> program_name
	argv++;

	// iterate over all the options (must be at the start)
	while (*argv && (flag_ret = is_valid_flag(*argv, args))) {
	    if (flag_ret == -1)
			break;
	    argv++;
	}

	if (flag_ret == -1) {
        parsing_error("Flag already used -> ", (char*)*argv);
        return 0;
	}

	if (!*argv) {
		parsing_error("invalid input", 0);
		usage();
		return 0;
	}

	if (!parse_address(argv, args))
	    return 0;

	return 1;

}
