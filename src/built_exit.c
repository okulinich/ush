#include "ush.h"

int mx_exit(t_lst *h) {
    int flag = 0;

    if (!h->av[1]) {
        //system("leaks -q ush");
        exit(0);
    }
    for (unsigned int i = 0; i < strlen(h->av[1]); i++)
        if (!isnumber(h->av[1][i])) {
            fprintf(stderr, "%s\nush: %s: %s: numeric argument required\n",
                    h->av[0], h->av[0], h->av[1]);
            exit(255);
        }
    if (mx_arr_size(h->av) > 2) {
        fprintf(stderr, "%s\nush: %s: too many arguments\n",
                h->av[0], h->av[0]); //  не делать exit
        flag = 1;
    }
    //system("leaks -q ush");
    if (!flag) {
        //system("leaks -q ush");
        exit(atoi(h->av[1]));
    }
    return 1;
}

// доделать ексит!
// если число 256 то выход == 1 если 255 == 0 267 == 2 и тд
// выходит не больше лонг лонг инта заходит туда
// сделать так что бы оно обрабатывао это
