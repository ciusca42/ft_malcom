#include "../../includes/ft_malcom.h"

int is_target_address() {
	return 0;
}

void handle_packet(int sockfd, struct sockaddr_ll *saddr, unsigned char *buffer, t_args args) {
	arp_pckt packet;
	reply_struct reply;

	if (saddr->sll_pkttype != PACKET_BROADCAST) return;

	
	if (args.verbose)
	print_sockaddr_ll(*saddr);

	/* --- extract header --- */
    packet = extract_arp_frame(buffer);
	if (memcmp(&packet.ar$spa, &args.target_ip, packet.ar$pln) != 0) {
		return;
	}
	info_log("recived an ARP packet from target: ");
	print_address(args.target_ip, 4, ".");

	if (args.verbose)
		print_arp_packet(packet);

	/* --- create reply --- */
    reply = create_reply(sockfd, args);

	if (send_reply(reply, args.verbose) == -1) {
		err_log("reply error");
		close(sockfd);
		exit(1);
	}
	info_log("reply sent");

	close(sockfd);
	exit(0);
}