#include "../../includes/ft_malcom.h"
#include <stdio.h>

void parsing_error(char *error_type, char *msg) {
    const char *reset = "\033[0m";
    const char *red = "\033[1;31m";
                //red [Error type]: reset *msg;
    fprintf(stderr, "\n[%s%s%s]: %s\n", red, error_type, reset, msg);
}
