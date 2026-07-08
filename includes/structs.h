#ifndef STRUCTS_H 
#define STRUCTS_H

typedef struct s_arp_packet {
	unsigned short ar$hrd[2]; /* Hardware address space (e.g., Ethernet,
			                    Packet Radio Net.). */

	unsigned short ar$pro[2]; /* Protocol address space.  For Ethernet
                                hardware, this is from the set of type
                                fields ether_typ$<protocol>. */

    unsigned char ar$hln; /* byte length of each hardware address */
    unsigned char ar$pln; /* byte length of each protocol address */

    unsigned short ar$op[2]; /* opcode (ares_op$REQUEST | ares_op$REPLY) */

    unsigned char ar$sha[6]; /* Hardware address of sender of this
					            packet, n from the ar$hln field. */

    unsigned char ar$spa[4]; /* Protocol address of sender of this
					            packet, m from the ar$pln field. */

    unsigned char ar$tha[6]; /* Hardware address of target of this
					            packet (if known).*/

    unsigned char ar$tpa[4]; /* Protocol address of target. */
} 	arp_pckt;

#endif