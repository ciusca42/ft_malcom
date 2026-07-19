#include "../../includes/ft_malcom.h"


void fill_addr(char **source, unsigned char dest[], int size) {

    for (int i = 0; i < size; i++) {
        if (size == 4)
            dest[i] = atoi(source[i]);
        else
            dest[i] = (unsigned char)strtoul(source[i], NULL, 16);
    }
}


size_t mat_len(char **mat) {
    size_t len;

    len = 0;
    while (mat[len])
        len++;
    return len;
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
    size_t len;
    char *pair;

   len = mat_len(mac_addr);

    if (len != 6)
        return 0;

    for (int i = 0; i < 6; i++) {
        pair = mac_addr[i];
        if (ft_strlen(pair) != 2)
            return 0;
        if (!is_valid_hex(pair))
            return 0;
    }
    return 1;
}


int try_parse(const char **argv, char sep,  int (*parse_addr)(char**), unsigned char args_addr[]) {

    size_t len;
    // char *error_type;
    char **addr;
    size_t addr_size;

    addr_size = sep == '.' ? 4 : 6;
    // error_type = sep == '.' ? "Invalid IP" : "Invalid MAC";

    len = ft_strlen(*argv);

    // account for the separator at the end or start of the string
    // ft_split will ignore those.
    if ( (*argv)[0] == sep || (*argv)[len - 1] == sep)  {
        // parsing_error(error_type,(char *)*argv);
        return 0;
    }

    addr = ft_split(*argv, sep);

    //parse ip or parse mac
    if (!parse_addr(addr)) {
        free_matrix(addr);
        return 0;
    }
    fill_addr(addr, args_addr, addr_size);
    free_matrix(addr);
    return 1;
}