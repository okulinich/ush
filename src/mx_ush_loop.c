#include "ush.h"

void mx_ush_loop(t_global *hd) {
    t_lst *root;
    int status = 1;
    t_cmd_history *hist = NULL;

    while (status) {
        hd->new = mx_ush_read_line(&hist, &hd);                     //зчитуємо строку
        root = hd->new;
        for ( ; hd->new; hd->new = hd->new->next) {
            printf("Cmd = %s\narg1 = %s\n", hd->new->cmd, hd->new->av[1]);
            status = mx_ush_execute(hd, hd->new);                 //виконуємо команди
        }
        delete_list(root);
    }
    delete_history(hist);
}
