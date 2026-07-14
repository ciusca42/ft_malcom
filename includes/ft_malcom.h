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

#define IP_ADDRESS -1
#define MAC_ADDRESS 1

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
#include <net/if.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <time.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <sys/wait.h>

// projects inludes
#include "errors.h"
#include "structs.h"

//libft
#include "../libft/libft.h"

/* parsing */
t_args parse_input(const char **argv);

/* parsing msgs */
void parsing_error(char *error_type, char *msg);

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
void print_sockaddr_ll(struct sockaddr_ll info);
void print_sockaddr(struct sockaddr *sa);

/* packet handler */
arp_pckt extract_arp_frame(unsigned char *buffer);
ssize_t send_reply(reply_struct reply, int verbose);
reply_struct create_reply(int sockfd, t_args args);
void handle_packet(int sockfd, struct sockaddr_ll *saddr, unsigned char *buffer, t_args args);

#endif //FT_MALCOM_FT_MALCOM_H
