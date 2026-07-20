/********************/
/*	made by ciusca  */
/********************/

/*
    [Usefule Links]

	ARP: RFC 826 https://datatracker.ietf.org/doc/html/rfc826;
	ETHERNET: https://www3.diism.unisi.it/~benelli/scienze_della_comunicazione/dispense/2006_07/Ethernet.pdf
*/

#include "../includes/ft_malcom.h"

int sockfd;

void stop_progrram(int sig) {

    (void)sig;
    // cancel the ^C characters wrote when sending SIGINT
    fprintf(stderr, "\b\b");
    info_log("Quitting program...");
    close(sockfd);
    sockfd = -1;
    // exit(0);
}


int main(const int argc, const char **argv)
{
	t_args args;
	socklen_t saddr_size;
    ssize_t data_size;
	struct sockaddr_ll saddr;
    // int sockfd;
	unsigned char *buffer;

	
	if (!parse_input(argv,argc, &args)) {
	    return 0;
	}

	if (args.verbose) {
	    print_input(args);
	}


	info_log("creating socket");
	sockfd = socket(AF_PACKET , SOCK_DGRAM , htons(ETH_P_ARP));

	if(sockfd < 0)
	{
        err_log("Socket");
		return 1;
	}


	buffer = (unsigned char *)malloc(64);
	info_log("socket created");
    saddr_size = sizeof saddr;
    signal(SIGINT, stop_progrram);
	while(1)
	{
		// printf("waiting for packet...");
		info_log("waiting for packets...");
		data_size = recvfrom(sockfd , buffer , 64 , 0 , (struct sockaddr*)&saddr , &saddr_size);
		if (sockfd == -1)
		    break;
		if( data_size < 0) {
			continue;
			warn_log("A packet was lost");
		}
        handle_packet(sockfd, &saddr, buffer, args);
	}

	if (sockfd != -1)
	    close(sockfd);
	free(buffer);
	printf("Finished");
	return 0;
}
