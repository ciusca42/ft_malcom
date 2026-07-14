/********************/
/*	made by ciusca  */
/********************/

/*
	useful links

	ARP: RFC 826 https://datatracker.ietf.org/doc/html/rfc826;

*/


#include "../includes/ft_malcom.h"
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>


int program_running = 1;



t_args fill_input(void) {
	// 00:0c:29:2b:b5:bf
	t_args args = {
		.source_mac = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16},
		.target_mac = {0x16, 0x5d, 0xa4, 0x4a, 0xa4, 0xf6},
		// .target_mac = {0x00, 0x0c, 0x29, 0x2b, 0xb5, 0xbf},
		.source_ip  = {10, 0, 0, 1},
		.target_ip  = {10, 0, 0, 2},
	};

	return args;

}





int get_interface_index(struct sockaddr *source_ip) {
	(void)source_ip;
	struct ifaddrs *ifap;
	int inf_addr;

	inf_addr = getifaddrs(&ifap);

	if (inf_addr == -1) {
		perror("getinfaddrs");
		exit(1);
	}

	while (ifap->ifa_next != NULL) {

		// skip mac addresses
		if (!ifap->ifa_netmask) {
			ifap = ifap->ifa_next;
			continue;
		}

		printf("interface name: %s\n", ifap->ifa_name);
		printf("ifa_flagas: %u\n", ifap->ifa_flags);
		printf("ifa_addr: ");
		print_sockaddr(ifap->ifa_addr);
		printf("netmask: ");
		print_sockaddr(ifap->ifa_netmask);
		printf("---------\n");
		ifap = ifap->ifa_next;
		// print_address(, sizeof(&ifap->ifa_netmask), ":");
	}
	return 1;
}


int main(const int argc, const char **argv)
{
	t_args args;
	socklen_t saddr_size;
    ssize_t data_size;
	struct sockaddr_ll saddr;
    int sockfd;
	unsigned char *buffer;

	// args = fill_input();

	// int interface_index = get_interface_index((struct sockaddr *)&saddr);

	if (argc < 2) {
		fprintf(stderr, "Invalid Arguments\n");
		usage();
		return 1;
	}

	// if (argc == 2) {

	// }

	if (argc < 5) {
		fprintf(stderr, "Invalid Arguments\n");
		usage();
		return 1;
	}
	
	args = parse_input(argv);

	printf("source_ip: ");
	print_address(args.source_ip, 4, ".");

	printf("source_mac: ");
	print_address(args.source_mac, 6, ":");

	printf("target_ip: ");
	print_address(args.target_ip, 4, ".");

	printf("target_mac: ");
	print_address(args.target_mac, 6, ":");
	
	
	buffer = (unsigned char *)malloc(64);
	// return parse_input(argv);
	// perror("test");

	info_log("creating socket");
	sockfd = socket(AF_PACKET , SOCK_DGRAM , htons(ETH_P_ARP));

	if(sockfd < 0)
	{
        err_log("Socket");
		return 1;
	}


	info_log("socket created");
    // saddr_size = sizeof saddr;
	while(program_running)
	{
		printf("waiting for packet...");
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
