#include "../../includes/ft_malcom.h"

void stop_program(int sig) {

    (void)sig;
    
    // cancel the ^C characters wrote when pressing Ctrl + C
    fprintf(stderr, "\b\b");
    
    info_log("Quitting program...");
    close(sockfd);
    sockfd = -1;
    // exit(0);
}
