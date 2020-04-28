#include "ush.h"

void mx_ush_loop(t_global *hd, char *input) {
    t_lst *root;
    int status = 1;
    t_cmd_history *hist = NULL;
    hd->last_exit_status = 0;

    while (status) {
        mx_handler();
        hd->new = mx_ush_read_line(&hist, hd, input);                     //зчитуємо строку
        root = hd->new;
        for ( ; hd->new; hd->new = hd->new->next) {
            status = mx_ush_execute(hd, hd->new);                 //виконуємо команди
        }
        delete_list(root);
        if(mx_strcmp("suka", input) != 0) {
            free(input);
            break;
        }
        // system("leaks -q ush");
        // // exit(1);
    }
    if(mx_strcmp(input, "suka") == 0)
        delete_history(hist);
}
