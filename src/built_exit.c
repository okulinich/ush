#include "ush.h"

void mx_exit(t_lst *h) {
    int flag = 0;

    if (!h->av[1])
        exit(0);
    for (unsigned int i = 0; i < strlen(h->av[1]); i++)
        if (!isnumber(h->av[1][i])) {
            fprintf(stderr, "%s\nu$h: %s: %s: numeric argument required\n",
                    h->av[0], h->av[0], h->av[1]);
            // free(h);
            exit(255);
        }
    if (mx_arr_size(h->av) > 2) {
        fprintf(stderr, "%s\nexit: too many arguments\n", h->av[0]); //  не делать exit
        // free(h);
        flag = 1;
    }
    system("leaks -q ush");
    if (!flag)
        exit(atoi(h->av[1]));
}
