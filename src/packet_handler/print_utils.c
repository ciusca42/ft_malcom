#include "../../includes/ft_malcom.h"


void print_sockaddr_ll(struct sockaddr_ll info) {
    printf("-----struct sockaddr_ll------\n");
    printf("sll_family:   %u\n", info.sll_family);
    printf("sll_protocol: %02x\n", info.sll_protocol);
    printf("sll_ifindex:  %d\n", info.sll_ifindex);
    printf("sll_hatype:   %u\n", info.sll_hatype);
    printf("sll_pkttype:  %u\n", info.sll_pkttype);
    printf("sll_halen:    %u\n", info.sll_halen);
    printf("sll_addr:     ");
    print_address(info.sll_addr, info.sll_halen, ":");
    printf("\n");
}

void print_address(unsigned char *addr, unsigned short len, const char *sep)
{
    for (int j = 0; j < len; j++)
    {
        if (sep[0] == ':')
            printf("%02x", addr[j]);
        else
            printf("%hu", addr[j]);
        if (j < len - 1)
            printf("%s", sep);
    }
    printf("\n");
}

void print_sockaddr(struct sockaddr *sa) {

	if (sa->sa_family == AF_INET) {
		struct sockaddr_in *sin = (struct sockaddr_in *)sa;
		char ip_str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &sin->sin_addr, ip_str, sizeof(ip_str));
		printf("AF_INET: %s\n", ip_str);
	}
	else {
		printf("not handled family (ipv4 only)\n");
	}
}

void print_arp_packet(arp_pckt packet)
{
    printf("-----ARP PACKET-----\n");
    printf("ar$hrd (hardware type): 0x%02x\n", packet.ar$hrd);
    printf("ar$pro (protocol type): 0x%02x\n", packet.ar$pro);
    printf("ar$hln (hw addr len):    %hu\n", packet.ar$hln);
    printf("ar$pln (proto addr len): %hu\n", packet.ar$pln);
    printf("ar$op (opcode):          %02x\n", packet.ar$op);

    printf("ar$sha (sender MAC):    ");
    print_address(packet.ar$sha, packet.ar$hln, ":");

    printf("ar$spa (sender IP):     ");
    print_address(packet.ar$spa, packet.ar$pln, ".");

    printf("ar$tha (target MAC):    ");
    print_address(packet.ar$tha, packet.ar$hln, ":");

    printf("ar$tpa (target IP):     ");
    print_address(packet.ar$tpa, packet.ar$pln, ".");
}
