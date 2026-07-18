#include "../../includes/ft_malcom.h"

// void activate_flag(char *flag) {

// }

int resolve_hostname(char *host) {
    (void)host;
    return 0;
}

size_t mat_len(char **mat) {
    size_t len;

    len = 0;
    while (mat[len])
        len++;
    return len;
}

int is_valid_flag(const char *flag, t_args *arg) {

	const char *valid_flags[] = {"-v", "--verbose", 0};

	for (int i = 0; valid_flags[i]; i++) {
		if (strcmp(valid_flags[i], flag) == 0) {
			if (i == 0 || i == 1)
				arg->verbose = 1;
			return 1;
		}
	}
	return 0;

}

int parse_ip(char **ip_addr) {
    int num;
    int i;
    size_t len;

    len = mat_len(ip_addr);
    if (len != 4)
        return 0;
    i = 0;
    while (ip_addr[i]) {
        if (ft_strlen(ip_addr[i]) > 3)
            return 0;
        num = ft_atoi(ip_addr[i]);
        if (n_len(num) != (int)ft_strlen(ip_addr[i]))
            return 0;
        //account for buffer overflow
        if (num == 0 && ft_strncmp("0", ip_addr[i], len) != 0)
            return 0;
        if (num == -1)
            return 0;
        i++;
    }
    return 1;
}

int is_valid_hex(char hex[2]) {
    char hex_chr;
    for (int i = 0; i < 2; i++) {
        hex_chr = ft_toupper(hex[i]);
        if (!ft_isdigit(hex_chr) && (hex_chr >  'F' || hex_chr < 'A'))
            return 0;
    }
    return 1;
}

// da 0 a 9 da A a F
int parse_mac(char **mac_addr) {
    size_t len = mat_len(mac_addr);
    if (len != 6)
        return 0;
    for (int i = 0; i < 6; i++) {
        char *pair = mac_addr[i];
        if (ft_strlen(pair) != 2)
            return 0;
        if (!is_valid_hex(pair))
            return 0; 
    }
    return 1;
}

void fill_addr(char **addr, unsigned char arg_addr[], int size) {
   
    for (int i = 0; i < size; i++) {
        if (size == 4)
            arg_addr[i] = atoi(addr[i]);
        else
            arg_addr[i] = (unsigned char)strtoul(addr[i], NULL, 16);
    }
}

int try_parse(const char **argv, char sep,  int (*fun)(char**), unsigned char args_addr[]) {
    
    size_t len;
    char *error_type;
    char **addr;
    size_t addr_size;
    // int addr_type;

    // addr_type = sep == '.' ? IP_ADDRESS : MAC_ADDRESS;
    addr_size = sep == '.' ? 4 : 6;
    error_type = sep == '.' ? "Invalid IP" : "Invalid MAC";
    
    len = ft_strlen(*argv);
    if (*argv[0] == sep || *argv[len - 1] == sep)  {
        parsing_error(error_type,(char *)*argv);
        return 0;
    }
    addr = ft_split(*argv, sep);
    if (!fun(addr)) {
        parsing_error(error_type, (char*)*argv);
        free_matrix(addr);
        return 0;
    }
    fill_addr(addr, args_addr, addr_size);
    free_matrix(addr);
    return 1;
}

int parse_address(const char **argv, t_args *args) {

    if (!try_parse(argv, '.', parse_ip, args->source_ip))
        return 0;
    argv++;
    if (!try_parse(argv, ':', parse_mac, args->source_mac))
        return 0;
    argv++;
    if (!try_parse(argv, '.', parse_ip, args->target_ip))
        return 0;
    argv++;
    if (!try_parse(argv, ':', parse_mac, args->target_mac))
        return 0;
   
    return 1;
}

t_args parse_input(const char **argv) {
	
	t_args args;
	// int i;
	args.verbose = 0;
	// i = 1;
	argv++;
	while (*argv && is_valid_flag(*argv, (t_args *)&args)) {
	    argv++;
	}

	if (!*argv) {
		parsing_error("Input error","invalid input");
		usage();
		exit(1);
	}

	// int parse_address(char **argv) => while ++*argv
	if (!parse_address(argv, (t_args *)&args))
	    exit(1);

	return args;
	
}


// int check_flags(char **argv) {

// 	for (int i = 1; argv[i]; i++) {
// 		if (!is_valid_flag(argv[i]))
// 			return 0;
// 	}
// 	return 1;

// 	if (strcmp(argv[1], "--help") == 0) {
// 		print_help();
// 		return 1;
// 	}
// 	return 0;

// }