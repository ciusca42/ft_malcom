#include "../../includes/ft_malcom.h"

void print_input(t_args args) {
    printf("recived input\n");
   	printf("source_ip: ");
   	print_address(args.source_ip, 4, ".");
   
   	printf("source_mac: ");
   	print_address(args.source_mac, 6, ":");
   
   	printf("target_ip: ");
   	print_address(args.target_ip, 4, ".");
   
   	printf("target_mac: ");
   	print_address(args.target_mac, 6, ":");
}