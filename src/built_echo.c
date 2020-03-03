#include "ush.h"

static char *search_for_var(t_global *hd, char *str) {
    for(int i = 0; hd->env[i]; i++) {
        if(find_var_in_str(hd->env[i], &str[1])) {
            return &hd->env[i][mx_strlen(str)];
        }
    }
    for(int i = 0; hd->vars[i]; i++) {
        if(find_var_in_str(hd->vars[i], &str[1])) {
            return &hd->vars[i][mx_strlen(str)];
        }
    }
    return NULL;
}


int mx_echo(t_global *hd, t_lst *head) {
    char *str;
    bool no_new_line = false;
    int i = 1;

    if(!head->av[1])
        mx_printstr("\n");
    else {
        if(mx_strcmp(head->av[1], "-n") == 0) {
            no_new_line = true;
            i++;
        }
        for(; head->av[i]; i++) {
            if(head->av[i][0] == '$') {
                str = search_for_var(hd, head->av[i]);
                if(str != NULL) {
                    mx_printstr(str);
                }
            }
            else {
                mx_printstr(head->av[i]);
            }
            if(head->av[i + 1])
                mx_printstr(" ");
        }
        if(no_new_line) {
            mx_printstr(MX_WHITE);
            mx_printstr("%");
            mx_printstr(MX_NORM);
        }
        mx_printstr("\n");
    }
    return 1;
}
