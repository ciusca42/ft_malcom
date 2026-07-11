#include "../../includes/ft_malcom.h"

arp_pckt extract_arp_frame(unsigned char *buffer) {
    arp_pckt packet;
    ssize_t i;
    i = 0;

	packet.ar$hrd = (((short)buffer[i]) << 8) | buffer[i + 1];
	i+= 2;

	packet.ar$pro = (((short)buffer[i]) << 8) | buffer[i + 1];
	i += 2;

	packet.ar$hln = buffer[i];
	i++;

	packet.ar$pln = buffer[i];
	i++;

	packet.ar$op = (((short)buffer[i]) << 8) | buffer[i + 1];
	i+=2;

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