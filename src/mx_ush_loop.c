#include "ush.h"

static bool semicol_error(char *str) {
    bool flag = true;

    if (mx_strlen(str) > 0) {
        for (int i = 0; i < mx_strlen(str); i++) {
            if (str[i] == ';' && flag) {
                mx_printerr("ush: syntax error near unexpected token `;\'\n");
                break;
            }
            if (isalpha(str[i])) {
                flag = false;
                break;
            }
        }
        return flag;
    }
    return flag;
}

void mx_ush_loop(t_global *hd) {
    t_lst *root;
    int status = 1;

    while (status) {
        if (!mx_string_has_chars(hd->input) || semicol_error(hd->input))
            break;
        hd->new = mx_ush_read_line(hd->input); //зчитуємо строку
        if (hd->new == NULL)
            continue;
        root = hd->new;
        signal(SIGINT, mx_handler);
        for ( ; hd->new; hd->new = hd->new->next)
            status = mx_ush_execute(hd, hd->new);  //виконуємо команди
        mx_delete_list(root);
        if (mx_strcmp("emptyinput", hd->input) != 0)
            break;
    }
    free(hd);
    free(hd->input);
    exit(0);
}
