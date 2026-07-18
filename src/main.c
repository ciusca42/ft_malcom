/********************/
/*	made by ciusca  */
/********************/

/*
    [Usefule Links]

	ARP: RFC 826 https://datatracker.ietf.org/doc/html/rfc826;
	ETHERNET: https://www3.diism.unisi.it/~benelli/scienze_della_comunicazione/dispense/2006_07/Ethernet.pdf
*/

#include "../includes/ft_malcom.h"

// int get_interface_index() {
// 	struct ifaddrs *ifap;
// 	int found;

// 	found = 0;

// 	if (getifaddrs(&ifap) == -1) {
// 		perror("getinfaddrs");
// 		exit(1);
// 	}

// 	while (ifap->ifa_next != NULL) {
	
// 	    if (!ifap->ifa_netmask && ifap->ifa_flags != 69699) continue;

// 		found++;
// 		printf("Network Interface Found!\n");	
// 		printf("interface name: %s\n", ifap->ifa_name);
// 		printf("address: ");
// 		print_sockaddr(ifap->ifa_addr);
// 		printf("netmask: ");
// 		if (ifap->ifa_netmask)
// 		    print_sockaddr(ifap->ifa_netmask);
// 		printf("---------\n");
// 		printf("\n");
// 		ifap = ifap->ifa_next;
// 		// print_address(, sizeof(&ifap->ifa_netmask), ":");
// 	}

// 	freeifaddrs(ifap);

// 	return 1;
// }


int main(const int argc, const char **argv)
{
	t_args args;
	socklen_t saddr_size;
    ssize_t data_size;
	struct sockaddr_ll saddr;
    int sockfd;
	unsigned char *buffer;

	printf("print mat\n");
	printf("mat[1] = %s\n", argv[1]);
	if (argc < 2) {
		fprintf(stderr, "Invalid Arguments\n");
		usage();
		return 1;
	}

	if (argc == 2 && !ft_strncmp(argv[1], "help", ft_strlen(argv[1]))) {
        print_help();
        return 0;
	}

	if (argc < 5) {
		fprintf(stderr, "Invalid Arguments\n");
		usage();
		return 1;
	}

	args = parse_input(argv);

	if (args.verbose) {
	    print_input(args);
	}

	buffer = (unsigned char *)malloc(64);

	info_log("creating socket");
	sockfd = socket(AF_PACKET , SOCK_DGRAM , htons(ETH_P_ARP));

	if(sockfd < 0)
	{
        err_log("Socket");
		return 1;
	}


	info_log("socket created");
    saddr_size = sizeof saddr;
	while(1)
	{
		// printf("waiting for packet...");
		info_log("waiting for packets...");
		data_size = recvfrom(sockfd , buffer , 64 , 0 , (struct sockaddr*)&saddr , &saddr_size);
		if( data_size < 0) {
			continue;
			warn_log("A packet was lost");
		}
        handle_packet(sockfd, &saddr, buffer, args);
	}
	close(sockfd);
	// free(buffer);
	printf("Finished");
	return 0;
}
