#ifndef STRUCTS_H 
#define STRUCTS_H

#include <stdio.h>



typedef struct s_arp_packet {
	unsigned short ar$hrd;   /* 16 bit Hardware address space (e.g., Ethernet,
			                    Packet Radio Net.). */

	unsigned short ar$pro; /* 16 bit Protocol address space.  For Ethernet
                                hardware, this is from the set of type
                                fields ether_typ$<protocol>. */

    unsigned char ar$hln; /* 8 bit byte length of each hardware address */
    unsigned char ar$pln; /* 8 bit byte length of each protocol address */


    unsigned short ar$op; /* 16 bit opcode (ares_op$REQUEST | ares_op$REPLY) */

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

    //addr??
    unsigned char source_ip[4];
    unsigned char source_mac[6];
    unsigned char target_ip[4];
    unsigned char target_mac[6];

} t_args;

#endif