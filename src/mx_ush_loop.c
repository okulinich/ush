#include "ush.h"

void mx_ush_loop(t_global *hd) {
    t_lst *root;
    int status = 1;

    while (status) {
        hd->new = mx_ush_read_line(hd->input); //зчитуємо строку
        if (hd->new == NULL)
            continue;
        root = hd->new;
        signal(SIGINT, mx_handler);
        for ( ; hd->new; hd->new = hd->new->next) {
            status = mx_ush_execute(hd, hd->new);  //виконуємо команди
        }
        mx_delete_list(root);
        if (mx_strcmp("emptyinput", hd->input) != 0)
            break;
    }
    free(hd);
    free(hd->input);
    exit(0);
}
