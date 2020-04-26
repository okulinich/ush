#include "ush.h"

int mx_exit(t_global *hd) {
    int flag = 0;

    if (!hd->new->av[1]) {
        // mx_del_strarr(&hd->env);
        mx_del_strarr(&hd->new->av);
        mx_strdel(&hd->new->cmd);
        free(hd);
        system("leaks -q ush");
        exit(0);
        return 1;
    }
    for (unsigned int i = 0; i < strlen(hd->new->av[1]); i++)
        if (!isnumber(hd->new->av[1][i])) {
            fprintf(stderr, "%s\nush: %s: %s: numeric argument required\n",
                    hd->new->av[0], hd->new->av[0], hd->new->av[1]);
            exit(255);
            return 1;
        }
    if (mx_arr_size(hd->new->av) > 2) {
        fprintf(stderr, "%s\nush: %s: too many arguments\n",
                hd->new->av[0], hd->new->av[0]); //  не делать exit
        flag = 1;
    }
    //system("leaks -q ush");
    if (!flag) {
        free(hd);
        system("leaks -q ush");
        exit(atoi(hd->new->av[1]));
    }
    return 1;
}

// доделать ексит!
// если число 256 то выход == 1 если 255 == 0 267 == 2 и тд
// выходит не больше лонг лонг инта заходит туда
// сделать так что бы оно обрабатывао это
