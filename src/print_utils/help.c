#include "../../includes/ft_malcom.h"



void print_help(void) {
    printf(BOLD "NAME" RESET "\n");
    printf("       ft_malcolm - send forged ARP replies to poison a target's ARP cache\n\n");

    printf(BOLD "USAGE" RESET "\n");
    printf("       ft_malcolm [" BOLD " options " RESET "] "
           "source_ip source_mac target_ip target_mac\n\n");

    printf(BOLD "DESCRIPTION" RESET "\n");
    printf("       Crafts and sends a spoofed ARP reply over a raw AF_PACKET socket,\n"
           "       telling the target that source_ip now lives at source_mac. This\n"
           "       can be used to redirect traffic destined for source_ip through the\n"
           "       attacker's machine (a classic ARP cache poisoning / MITM setup).\n\n");

    printf(BOLD "OPTIONS" RESET "\n");
    printf("       " BOLD "-v" RESET ", " BOLD "--verbose" RESET "\n"
           "              Print detailed information about each packet sent,\n"
           "              including the raw header fields and sender address.\n\n");

    printf("       " BOLD "help" RESET "\n"
           "              Display this help message and exit.\n\n");

    printf(BOLD "ARGUMENTS" RESET "\n");
    printf("       " BOLD "source_ip" RESET "\n"
           "              The IP address the ARP reply claims to belong to. This is\n"
           "              the identity being spoofed — the target will believe this\n"
           "              IP resolves to source_mac after receiving the reply.\n\n");

    printf("       " BOLD "source_mac" RESET "\n"
           "              The (spoofed) MAC address sent back in the reply, bound\n"
           "              to source_ip. Typically the attacker's own interface MAC.\n\n");

    printf("       " BOLD "target_ip" RESET "\n"
           "              The IP address of the machine that will receive the\n"
           "              forged reply — i.e. the sender/victim whose ARP cache\n"
           "              is being poisoned.\n\n");

    printf("       " BOLD "target_mac" RESET "\n"
           "              The real MAC address of the target machine (the sender),\n"
           "              used to address the reply directly to it.\n\n");

    printf(BOLD "EXAMPLES" RESET "\n");
    printf("       ft_malcolm -v 192.168.1.1 aa:bb:cc:dd:ee:ff 192.168.1.42 11:22:33:44:55:66\n\n");
}

void usage() {
    printf("\n"BOLD"usage"RESET": ./ft_malcom [ "BOLD"options"RESET" ] source_ip, source_mac, target_ip, target_mac\n");
    printf("use " BOLD"help"RESET" for more informations\n\n");
}