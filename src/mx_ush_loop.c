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
            if (mx_strcmp(hd->new->cmd, "exit") == 0)
                free(input);
            status = mx_ush_execute(hd, hd->new);                 //виконуємо команди
        }
        delete_list(root);
        if(mx_strcmp("emptyinput", input) != 0) {
            break;
        }
        // system("leaks -q ush");
        // // exit(1);
    }
    free(hd);
    free(input);
    if(mx_strcmp(input, "emptyinput") == 0)
        delete_history(hist);
    //system("leaks -q ush");
    exit(0);
}
