#include "../../includes/ft_malcom.h"

int resolve_hostname(char *hostname, unsigned char *arg_ip) {
    // info_log("tryng to resolve...\n");

    struct addrinfo options, *result;
    struct sockaddr_in *hostname_ip;
    char ip_string[INET_ADDRSTRLEN];
    char **splitted;

    memset(&options, 0, sizeof(options));
    options.ai_family = AF_INET;
    options.ai_socktype = SOCK_DGRAM;

    info_log("resolving hostname...");

    if (getaddrinfo(hostname, NULL, &options, &result) != 0) {
        // parsing_error("Hostname error", hostname);
        return 0;
    }

    // info_log(char *msg)
    hostname_ip = (struct sockaddr_in *)result->ai_addr;

    inet_ntop(AF_INET, &hostname_ip->sin_addr, ip_string, sizeof(ip_string));

    freeaddrinfo(result);

    splitted = ft_split(ip_string, '.');

    fill_addr(splitted, arg_ip, 4);
    free_matrix(splitted);
    info_log("hostname resolved");

    return 1;
}