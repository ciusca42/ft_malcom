#include "../../includes/ft_malcom.h"
#include <stdio.h>



void logger(char *msg, int type) {

    if (!VERBOSE)
        return;
    char *prefix;
    char *color;
    FILE *fd;
    char timestamp[32];
    char *reset = "\033[0m";
    struct timespec ts;
    struct tm tm_info;
    // int ms;
    size_t len;

    clock_gettime(CLOCK_REALTIME, &ts);
    gmtime_r(&ts.tv_sec, &tm_info);
    
    len = strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_info);
    snprintf(timestamp + len, sizeof(timestamp) - len, ".%05ld", ts.tv_nsec / 10000);

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
        break;
    default:
        return;
    }
    fprintf(fd, "%s%s%s-[%s%s%s]: %s\n", color, prefix, reset, "\033[0;35m", timestamp, reset, msg);
    if (type == ERROR) {
        fprintf(fd, "%s%s%s: ", color, "[Error Info]", reset);
        perror(msg);
    }

}


//utilities for the logger function
void info_log(char *msg) {
    logger(msg, INFO);
}

void warn_log(char *msg) {
    logger(msg, WARNING);
}

void err_log(char *msg) {
    logger(msg, ERROR);
}



