#include "ush.h"

void mx_ush_loop(t_global *hd) {
    t_lst *root;
    int status = 1;
    t_cmd_history *hist = NULL;
    hd->last_exit_status = 0;

    while (status) {
        hd->env = mx_env_copy();
        mx_handler();
        hd->new = mx_ush_read_line(&hist);                     //зчитуємо строку
        root = hd->new;
        for ( ; hd->new; hd->new = hd->new->next) {
            status = mx_ush_execute(hd, hd->new);                 //виконуємо команди
        }
        // system("leaks -q ush");
        // // exit(1);
       // free(root->cmd);
       // free(root->av);
       delete_list(root);
       mx_del_strarr(&hd->env);
    }
    delete_history(hist);
}
