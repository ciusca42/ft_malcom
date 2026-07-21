#include "../../includes/ft_malcom.h"

int is_target(t_args args, arp_pckt packet) {
	if (memcmp(&packet.ar$spa, &args.target_ip, packet.ar$pln) != 0 ||
	    memcmp(&packet.ar$sha, &args.target_mac, packet.ar$hln) != 0)
	{				
		return 0;
	}
	
	return 1;
}

void handle_packet(int sockfd, struct sockaddr_ll *saddr, unsigned char *buffer, t_args args) {
	arp_pckt packet;
	reply_struct reply;

	if (saddr->sll_pkttype != PACKET_BROADCAST) {
	    // info_log("not a broadcast packet");
	    return;
	}
    packet = extract_arp_frame(buffer);
	if (!is_target(args, packet)) {
		return;
	}

	
	if (args.verbose)
	    print_sockaddr_ll(*saddr);

	if (args.verbose)
		print_arp_packet(packet);
	
	info_log("recived an ARP packet from target: ");
	print_address(args.target_ip, 4, ".");

    reply = create_reply(sockfd, args);
    
	if (send_reply(reply, args.verbose, saddr->sll_ifindex) == -1) {
		err_log("sendto() error");
		close(sockfd);
		exit(1);
	}
	info_log("reply sent");
	if (!args.persist) {
	    close(sockfd);
		free(buffer);
		info_log("Quitting...");
		exit(0);
	}
	info_log("Waiting for packets...");
}