#include "ush.h"

int mx_set(t_lst *s) {
    int size = mx_arr_size(s->av);
    char **set;

    if (size > 1) {
        fprintf(stderr, "set: command does not accept any options\n");
        return -1;
    }
    else {
        set = mx_env_copy();
        mx_print_strarr(set, "\n");
    }
    mx_del_strarr(&set);
    return 1;
}

int mx_bye(t_global *g, t_lst *h) {
    return mx_exit(g, h);
}

int mx_call_true() {
    return 1;
}

int mx_call_false() {
    return -1;
}
