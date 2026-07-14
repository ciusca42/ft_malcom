#include "../../includes/ft_malcom.h"

arp_pckt extract_arp_frame(unsigned char *buffer) {
    arp_pckt packet;

    memcpy(&packet, buffer, sizeof(arp_pckt));

    packet.ar$hrd = ntohs(packet.ar$hrd);
    packet.ar$pro = ntohs(packet.ar$pro);
    packet.ar$op  = ntohs(packet.ar$op);

    return packet;
}