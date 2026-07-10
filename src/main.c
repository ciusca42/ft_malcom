#include "../includes/ft_malcom.h"
#include <sys/socket.h>

// #include <netinet/in.h>

int program_running = 1;

/*

struct sockaddr_ll {
	unsigned short sll_family;   // Always AF_PACKET 
	unsigned short sll_protocol; // Physical-layer protocol 
	int            sll_ifindex;  // Interface number 
	unsigned short sll_hatype;   // ARP hardware type 
	unsigned char  sll_pkttype;  // Packet type 
	unsigned char  sll_halen;    // Size of address 
	unsigned char  sll_addr[8];  // Physical-layer address 
};

*/


// int send_reply(reply_struct reply_pckt, arp_pckt packet) {
// 	sendto(int fd, const void *buf, size_t n, int flags, const struct sockaddr *addr, socklen_t addr_len)
// }


void handle_packet(int sockfd, struct sockaddr_ll *saddr, unsigned char *buffer) {
	arp_pckt packet;
	// reply_struct reply;
	(void)sockfd;
    info_log("parsing packet");
	printf("pkttype: %u\n", saddr->sll_pkttype); // 1 is broadcast i think, did not remember
	printf("struct_ll addre: \n");
	print_address(saddr->sll_addr, saddr->sll_halen, ":");
	printf("\n");
	packet = extract_arp_frame(buffer);
	print_arp_packet(packet);
	
	// reply.sockfd = sockfd;

	// send_reply(reply, packet);
	// send_reply()

    // debug_print(saddr);
}



int main()
{
	socklen_t saddr_size;
    ssize_t data_size;
	struct sockaddr_ll saddr;
    int sockfd;
	unsigned char *buffer = (unsigned char *)malloc(64);


	// printf("test libft: %zu\n",ft_strlen("ciaooo"));
	// if (argc < 2) {
	// 	fprintf(stderr, "Invalid Arguments\n");
	// 	usage();
	// 	return 1;
	// }

	// if (!check_flags(argv)) {
	// 	return 0;
	// } 

	// if (argc < 5) {
	// 	fprintf(stderr, "Invalid Arguments\n");
	// 	usage();
	// 	return 1;
	// }

	// return parse_input(argv);

	info_log("creating socket");
	sockfd = socket(AF_PACKET , SOCK_DGRAM , htons(ETH_P_ARP));
	if(sockfd < 0)
	{
        err_log("Socket");
		return 1;
	}
	info_log("socket created");

    saddr_size = sizeof saddr;
	info_log("start to receive incoming packets");
	while(program_running)
	{
		data_size = recvfrom(sockfd , buffer , 64 , 0 , (struct sockaddr*)&saddr , &saddr_size);
		if( data_size < 0)
			warn_log("A packet was lost");
		
		info_log("recived an ARP packet");
        handle_packet(sockfd, &saddr, buffer);
	}
	close(sockfd);
	// free(buffer);
	printf("Finished");
	return 0;
}

