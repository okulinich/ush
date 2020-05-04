#include "ush.h"

int mx_call_true() {
    return 1;
}

int mx_call_false() {
    return -1;
}

static void print_inside_help(char **com, int j) {
    printf("u$h: \033[1;31m%s:\033[0m ", com[j]);
    printf("\033[1;32m%s analog bash-3.2 command\033[0m\n", com[j]);
}

static int check_com(char *commands[], t_lst *l, bool find, int count) {
        for (int i = 1; l->av[i]; i++) {
            find = false;
            for(int j = 0; commands[j]; j++)
                if (mx_strcmp(l->av[i], commands[j]) == 0) {
                    print_inside_help(commands, j);
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
        printf("\tThis is bash analog program!\n\tTo wiew other commands type:");
        printf(" \033[1;31mhelp\033[0m \033[1;33m[command]\033[0m\n");
        count++;
    }
    else
        count = check_com(commands, l, find, count);
    g->last_exit_status = count > 0 ? 0 : 1;
    return 1;
}
