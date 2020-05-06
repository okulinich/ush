#include "ush.h"

void mx_ush_loop(t_global *hd) {
    t_lst *root;
    int status = 1;
    hd->last_exit_status = 0;

    while (status) {
        hd->new = mx_ush_read_line(hd, hd->input);                     //зчитуємо строку
        if(hd->new == NULL)
            continue;
        root = hd->new;
        signal(SIGINT, mx_handler);
        for ( ; hd->new; hd->new = hd->new->next) {
            // if (mx_strcmp(hd->new->cmd, "exit") == 0)
                // free(input);
            status = mx_ush_execute(hd, hd->new);                 //виконуємо команди
        }
        delete_list(root);
        if(mx_strcmp("emptyinput", hd->input) != 0) {
            break;
        }
        // system("leaks -q ush");
        // // exit(1);
    }
    free(hd);
    free(hd->input);
    //system("leaks -q ush");
    exit(0);
}
