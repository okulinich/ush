#include "ush.h"

int mx_call_true() {
    return 1;
}

int mx_call_false() {
    return -1;
}

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

static int check_com(char *commands[], t_lst *l, bool find, int count) {
    for (int i = 1; l->av[i]; i++) {
        find = false;
        for(int j = 0; commands[j]; j++)
            if (mx_strcmp(l->av[i], commands[j]) == 0) {
                printf("u$h: \033[1;31m%s:\033[0m ", commands[j]);
                printf("\033[1;32m%s - analog bash-3.2 command\033[0m\n", commands[j]);
                count++;
                find = true;
            }
        if (!find) {
            printf("u$h: help: no help topics match ");
            printf("%s\n", l->av[i]);
        }
    }
    return count;
}

int mx_help_command(t_global *g, t_lst *l) {
    int count = 0;
    bool find = false;
    char *commands[] = {"env", "export", "set", "unset", "pwd", "exit", "cd",
                   "which", "echo", "true", "false", "help", "return", NULL};
    if (l->cmd[0] && !l->av[1]) {
        printf("This is bash analog program!\nTo wiew other commands type:");
        printf(" \033[1;31mhelp\033[0m \033[1;33m[command]\033[0m\n");
        count++;
    }
    else
        count = check_com(commands, l, find, count);
    g->last_exit_status = count > 0 ? 0 : 1;
    return 1;
}
