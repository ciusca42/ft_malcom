#include "../../includes/ft_malcom.h"




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
}

void print_arp_packet(arp_pckt packet)
{
    printf("ar$hrd (hardware type): 0x%02x\n", packet.ar$hrd);
    printf("ar$pro (protocol type): 0x%02x\n", packet.ar$pro);
    printf("ar$hln (hw addr len):    %hu\n", packet.ar$hln);
    printf("ar$pln (proto addr len): %hu\n", packet.ar$pln);
    printf("ar$op (opcode):          %02x%02x\n", packet.ar$op[0], packet.ar$op[1]);

    printf("ar$sha (sender MAC):    ");
    print_address(packet.ar$sha, packet.ar$hln, ":");
    printf("\n");

    printf("ar$spa (sender IP):     ");
    print_address(packet.ar$spa, packet.ar$pln, ".");
    printf("\n");

    printf("ar$tha (target MAC):    ");
    print_address(packet.ar$tha, packet.ar$hln, ":");
    printf("\n");

    printf("ar$tpa (target IP):     ");
    print_address(packet.ar$tpa, packet.ar$pln, ".");
    printf("\n");
}
