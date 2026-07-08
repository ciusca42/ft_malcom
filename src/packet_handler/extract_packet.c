#include "../../includes/ft_malcom.h"

arp_pckt extract_arp_frame(unsigned char *buffer) {
    arp_pckt packet;
    ssize_t i;
    i = 0;
    


	for (int j = 0; j < 2; j++, i++)
        packet.ar$hrd[j] = buffer[i];

	for (int j = 0; j < 2; j++, i++)
        packet.ar$pro[j] = buffer[i];

	packet.ar$hln = buffer[i];
	i++;

	packet.ar$pln = buffer[i];
	i++;

	for (int j = 0; j < 2; j++, i++)
        packet.ar$op[j] = buffer[i];

	for (int j = 0; j < packet.ar$hln; j++, i++) {
        packet.ar$sha[j] = buffer[i];
	}

	for (int j = 0; j < packet.ar$pln; j++, i++) {
        packet.ar$spa[j] = buffer[i];
	}

	for (int j = 0; j < packet.ar$hln; j++, i++) {
        packet.ar$tha[j] = buffer[i];
	}

	for (int j = 0; j < packet.ar$pln; j++, i++) {
        packet.ar$tpa[j] = buffer[i];
	}
    return packet;
}