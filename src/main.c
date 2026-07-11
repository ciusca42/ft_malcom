/********************/
/*	made by ciusca  */
/********************/

/*
	useful links

	ARP: RFC 826 https://datatracker.ietf.org/doc/html/rfc826;

*/ 


#include "../includes/ft_malcom.h"
#include <linux/if_packet.h>
#include <stdio.h>
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



void handle_packet(int sockfd, struct sockaddr_ll *saddr, unsigned char *buffer) {
	arp_pckt packet;
	reply_struct reply;
	char print_str[100];
	//todo, maybe filter this on sockeoptions??
	if (saddr->sll_pkttype != PACKET_BROADCAST) return;
	info_log("recived an ARP packet");
    info_log("parsing packet");
	sprintf(print_str,"pkttype: %u\n", saddr->sll_pkttype);
	info_log(print_str); // 1 is broadcast i think, did not remember
	printf("protocol: %u\n", saddr->sll_protocol);
	info_log("struct_ll address: ");
	print_address(saddr->sll_addr, saddr->sll_halen, ":");
	printf("\n");
	info_log("extract ethernet payload");
	packet = extract_arp_frame(buffer);
	print_arp_packet(packet);
	
	unsigned char ip[] = {10, 0, 0, 2};


	// ip[0] = 10;
	// ip[1] = 0;
	// ip[2] = 0;
	// ip[3] = 2;


	if (memcmp(&packet.ar$spa, &ip, packet.ar$pln) == 0) {
		info_log("recived packet from target");
	}
	printf("print test ip\n");
	// print_address(ip, 4, ".");
	printf("\n");
	reply.sockfd = sockfd;


	// truct ifaddrs {
	// struct ifaddrs  *ifa_next;    /* Next item in list */
	// char            *ifa_name;    /* Name of interface */
	// unsigned int     ifa_flags;   /* Flags from SIOCGIFFLAGS */
	// struct sockaddr *ifa_addr;    /* Address of interface */
	// struct sockaddr *ifa_netmask; /* Netmask of interface */
	// }

	(void)reply;
	
	// send_reply(reply, packet);
	// send_reply()

    // debug_print(saddr);``
}

void wait_for_packet(int sockfd) {
    const char *frames[] = {".", "..", "...", ""};
    int frame = 0;
    fd_set readfds;
    struct timeval tv;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        tv.tv_sec = 0;
        tv.tv_usec = 300000; // 300ms

        int ret = select(sockfd + 1, &readfds, NULL, NULL, &tv);

        if (ret < 0) {
            perror("select");
            return;
        }

        if (ret == 0) {
            // timed out, no data yet -> advance animation
            printf("\rWaiting for packet%-4s", frames[frame % 4]);
            fflush(stdout);
            frame++;
            continue;
        }

        // ret > 0 -> data is ready to read
        if (FD_ISSET(sockfd, &readfds)) {
            printf("\rPacket received!         \n");
            return; // caller does the actual recvfrom() now
        }
    }
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

	// struct ifaddrs *ifap;
	// int inf_addr;
	// inf_addr = getifaddrs(&ifap);
	// if (inf_addr == -1) {
	// 	perror("getinfaddrs");
	// 	exit(1);
	// }

	// char *log_msg = malloc(1000);
	// while (ifap->ifa_next != NULL) {
	// 	sprintf(log_msg, "interface name: %s\n", ifap->ifa_name);
	// 	info_log(log_msg);
	// 	sprintf(log_msg, "ifa_flagas: %u", ifap->ifa_flags);
	// 	info_log(log_msg);
	// 	ifap = ifap->ifa_next;
	// 	// print_address(, sizeof(&ifap->ifa_netmask), ":");
	// }


	info_log("creating socket");
	sockfd = socket(AF_PACKET , SOCK_DGRAM , htons(ETH_P_ARP));
	if(sockfd < 0)
	{
		// perror("socket");
        err_log("Socket");
		return 1;
	}
	info_log("socket created");
    saddr_size = sizeof saddr;
	info_log("waiting for incoming packets");
	while(program_running)
	{
		wait_for_packet(sockfd);
		data_size = recvfrom(sockfd , buffer , 64 , 0 , (struct sockaddr*)&saddr , &saddr_size);
		if( data_size < 0) {
			continue;
			warn_log("A packet was lost");
		}
        handle_packet(sockfd, &saddr, buffer);
	}
	close(sockfd);
	// free(buffer);
	printf("Finished");
	return 0;
}

