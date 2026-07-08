#include "../../includes/ft_malcom.h"


#define ERROR_PREFIX "[ERROR] - "


void logger(char *msg, int type) {

    if (!VERBOSE)
        return;
    char *prefix;
    char *color;
    FILE *fd;
    char *timestamp = "0000"; // todo replace with actual timestamp
    char *reset = "\033[0m";

    fd = stdout;

    switch (type) {
    case INFO:
        prefix = "[INFO]";
        color = "\033[1;34m"; // blue
        break;
    case WARNING:
        prefix = "[WARNING]";
        color = "\033[1;33m"; // yellow
        break;
    case ERROR:
        prefix = "[ERROR]";
        color = "\033[1;31m"; // red
        fd = stderr;
    default:
        return;
    }
    
    fprintf(fd, "%s%s%s-[%s] %s\n", color, prefix, reset, timestamp, msg);
    if (type == ERROR)
        perror(msg);

}


//utilities for the logger function
void infolog(char *msg) {
    logger(msg, INFO);
}

void warnlog(char *msg) {
    logger(msg, WARNING);
}

void errlog(char *msg) {
    logger(msg, ERROR);
}



