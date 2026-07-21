#include "../../includes/ft_malcom.h"
// #include <net/if.h>

reply_struct create_reply(int sockfd, t_args args) {
    reply_struct reply;
    arp_pckt packet;

    // arp packet creation
    info_log("creating reply packet");
    packet.ar$hrd = htons(ARPHRD_ETHER);
    packet.ar$pro = htons(ETH_P_IP);
    packet.ar$hln = 6;
    packet.ar$pln = 4;
    packet.ar$op = htons(ARPOP_REPLY);
    memcpy(packet.ar$sha, args.source_mac, 6);
    memcpy(packet.ar$spa, args.source_ip, 4);
    memcpy(packet.ar$tha, args.target_mac, 6);
    memcpy(packet.ar$tpa, args.target_ip, 4);
    print_arp_packet(packet);
    reply.sockfd = sockfd; //todo replace with sockfd
    memcpy(reply.buffer, &packet, sizeof(packet));
    reply.buff_size = sizeof(arp_pckt);
    memcpy(reply.sender_addr, packet.ar$tha, packet.ar$hln);

    return reply;
}

ssize_t send_reply(reply_struct reply, int verbose, int nic) {
    ssize_t bytes_sent;
    struct sockaddr_ll header;

    memset(&header, 0, sizeof(header));
    header.sll_family = AF_PACKET;
    header.sll_protocol = htons(ETH_P_ARP);
    header.sll_ifindex = nic;
    header.sll_halen = 6;
    header.sll_hatype = 0;
    memcpy(header.sll_addr, reply.sender_addr, 6);
    header.sll_pkttype = 0;

    if (verbose) {
        print_sockaddr_ll(header);
        printf("\nsender addr (MAC): ");
        print_address(reply.sender_addr, 6, ":");
    }
    info_log("sending reply");
    bytes_sent = sendto(reply.sockfd,
            reply.buffer,
            reply.buff_size,
            0,
            (struct sockaddr *)&header,
            sizeof header);
    return bytes_sent;
}
