#ifndef STRUCTS_H 
#define STRUCTS_H

#include <stdio.h>

// see RFC 826 https://datatracker.ietf.org/doc/html/rfc826

typedef struct s_arp_packet {
	unsigned short ar$hrd; /* Hardware address space (e.g., Ethernet,
			                    Packet Radio Net.). */

	unsigned short ar$pro; /* Protocol address space.  For Ethernet
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


typedef struct s_reply_struct {

    int sockfd; // your socket
    char *buffer; // bytes buffer (to convert from arp_pckt)
    size_t buff_size; // buffer size
    struct sockaddr *addr; // sender addr   (addr.family / addr.address[addr_len] )
    size_t addr_len; // addr len

} reply_struct;


typedef struct s_args {

    char *source_ip;
    char *target_ip;
    char *source_mac;
    char *target_mac;

} t_args;

#endif