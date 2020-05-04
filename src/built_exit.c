#include "ush.h"

int mx_exit(t_global *hd, t_lst *l) {
    int flag = 0;

    if (!hd->new->av[1]) {
        //mx_del_strarr(&hd->env);
        mx_del_strarr(&hd->new->av);
        mx_strdel(&hd->new->cmd);
        free(hd);
        system("leaks -q ush");
        exit(0);
        return 1;
    }
    for (unsigned int i = 0; i < strlen(l->av[1]); i++)
        if (!isnumber(l->av[1][i])) {
            fprintf(stderr, "%s\nush: %s: %s: numeric argument required\n",
                    l->av[0], l->av[0], l->av[1]);
            exit(255);
            return 1;
        }
    if (mx_arr_size(l->av) > 2) { // ne delat exit
        fprintf(stderr,"%s\nush: %s: too many arguments\n", l->av[0], l->av[0]);
        flag = 1;
    }
    if (!flag) {
        free(hd);
        system("leaks -q ush");
        exit(atoi(hd->new->av[1]));
    }
    return 1;
}
