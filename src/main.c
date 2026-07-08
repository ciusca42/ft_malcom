#include "../includes/ft_malcom.h"
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <strings.h>
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



void handle_packet(struct sockaddr_ll *saddr, unsigned char *buffer) {
	arp_pckt packet;

    infolog("parsing packet");
	
	// printf("pkttype: %u\n", saddr->sll_pkttype); // 1 is broadcast i think, did not remember
	
	packet = extract_arp_frame(buffer);
	print_arp_packet(packet);

    // debug_print(saddr);
}


int main()
{
	socklen_t saddr_size;
    ssize_t data_size;
	struct sockaddr_ll saddr;
    int sockfd;
	unsigned char *buffer = (unsigned char *)malloc(64);

	infolog("creating socket");
	sockfd = socket(AF_PACKET , SOCK_DGRAM , htons(ETH_P_ARP));
	if(sockfd < 0)
	{
        errlog("Socket");
		return 1;
	}
	infolog("socket created");

    saddr_size = sizeof saddr;
	infolog("start to receive incoming packets");
	while(program_running)
	{
		data_size = recvfrom(sockfd , buffer , 64 , 0 , (struct sockaddr*)&saddr , &saddr_size);
		if( data_size < 0)
			warnlog("A packet was lost");
		
		infolog("recived an ARP packet");
        handle_packet(&saddr, buffer);
	}
	close(sockfd);
	// free(buffer);
	printf("Finished");
	return 0;
}