//
// Created by ciuschi on 7/5/26.
//

#ifndef FT_MALCOM_FT_MALCOM_H
#define FT_MALCOM_FT_MALCOM_H

#define DEBUG 1
#define VERBOSE 1

#define INFO 1 
#define WARNING 2 
#define ERROR 3

extern int g_signal;

// std library
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <linux/if_ether.h>

// projects inludes
#include "errors.h"
#include "structs.h"

//libft
#include "../libft/libft.h"

/* display help */
void print_help(void);
void usage();

/* logger */
void logger(char *msg, int type);
void info_log(char *msg);
void warn_log(char *msg);
void err_log(char *msg);

/* packet print utils */
void print_address(unsigned char *addr, unsigned short len, const char *sep);
void print_arp_packet(arp_pckt packet);

/* packet handler */
arp_pckt extract_arp_frame(unsigned char *buffer);
int send_packet(reply_struct reply);

#endif //FT_MALCOM_FT_MALCOM_H
