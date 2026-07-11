#include "../../includes/ft_malcom.h" 
#include <net/if.h>


int send_reply(reply_struct reply, arp_pckt packet) {
    
    ssize_t bytes_sent;
    struct sockaddr_ll header;

    (void)header;
    (void)packet;
    header.sll_family = AF_PACKET;
    header.sll_protocol = ntohs(ETH_P_ARP);
    header.sll_ifindex = if_nametoindex("eth0");




    bytes_sent = sendto(reply.sockfd, 
            reply.buffer,
            reply.buff_size,
            0,
            reply.addr,
            reply.addr_len);
    
    return bytes_sent;
}
