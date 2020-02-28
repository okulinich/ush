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


int mx_echo(t_global *hd) {
    char *str;

    if(!hd->new->av[1])
        mx_printstr("\n");
    else {
        for(int i = 1; hd->new->av[i]; i++) {
            if(hd->new->av[i][0] == '$') {
                str = search_for_var(hd, hd->new->av[i]);
                if(str != NULL) {
                    mx_printstr(str);
                }
            }
            else {
                mx_printstr(hd->new->av[i]);
            }
            if(hd->new->av[i + 1])
                mx_printstr(" ");
        }
        mx_printstr("\n");
    }
    return 1;
}

