#include "../../includes/ft_malcom.h"

void parsing_error(char *msg, char *var) {
    const char *reset = "\033[0m";
    const char *red = "\033[1;31m";

    if (!var)
        var = "";
                //red [Error type]: reset *msg;
    fprintf(stderr, "\n[%s%s%s]: %s%s\n", red, "ERROR", reset, msg, var);
}
