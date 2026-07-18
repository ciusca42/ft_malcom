#include "../../includes/ft_malcom.h"

int is_target_address() {
	return 0;
}

void handle_packet(int sockfd, struct sockaddr_ll *saddr, unsigned char *buffer, t_args args) {
	arp_pckt packet;
	reply_struct reply;

	if (saddr->sll_pkttype != PACKET_BROADCAST) {
	    info_log("not a broadcast packet");
	    return;
	}

	
	if (args.verbose)
	    print_sockaddr_ll(*saddr);

	/* --- extract header --- */
    packet = extract_arp_frame(buffer);
	if (args.verbose)
		print_arp_packet(packet);
	if (memcmp(&packet.ar$spa, &args.target_ip, packet.ar$pln) != 0) {
	    printf("returning here...\n");
		return;
	}
	info_log("recived an ARP packet from target: ");
	print_address(args.target_ip, 4, ".");


	/* --- create reply --- */
    reply = create_reply(sockfd, args);
    
	if (send_reply(reply, args.verbose, saddr->sll_ifindex) == -1) {
		err_log("sendto() error");
		close(sockfd);
		exit(1);
	}
	info_log("reply sent");
	close(sockfd);
	free(buffer);
	exit(0);
}