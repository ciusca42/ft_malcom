#include "../../includes/ft_malcom.h"


int is_valid_flag(const char *flag, t_args *arg) {
    t_flags table[] = {
        {"-v", "--verbose", &arg->verbose},
        {"-p", "--persist", &arg->persist},
        {NULL, NULL, NULL}
    };
    for (int i = 0; table[i].long_name; i++) {
        if (strcmp(table[i].short_name, flag) == 0 ||
            strcmp(table[i].long_name, flag) == 0) {
            if (*table[i].target == 1) {
                return -1;
            }
            *table[i].target = 1;
            return 1;
        }
    }
    return 0;
}