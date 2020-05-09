#include "ush.h"

static char *get_command_output(t_spawn_args *spwn, t_global *hd) {
    char *str = mx_strnew(1024);

    if (spwn->status != -1)
        hd->last_exit_status = spwn->status;
    else
        perror("waitpid");
    close(spwn->fds[1]);
    for (int i = 0; read(spwn->fds[0], &str[i], 1) > 0; i++)  
        ;
    str[mx_strlen(str) - 1] = '\0';
    close(spwn->fds[0]);
    return str;
}

char *mx_copy_echo_arg(t_lst *head, t_lst *root, int arg) {
    char *str = mx_strnew(strlen(head->av[arg]));
    
    mx_strcpy(str, head->av[arg]);
    mx_delete_list(root);
    return str;
}

void mx_create_new_proccess(t_spawn_args *spwn, t_lst *head) {
    extern char **environ;

    pipe(spwn->fds);
    posix_spawn_file_actions_init(&spwn->actions);
    posix_spawn_file_actions_adddup2(&spwn->actions, spwn->fds[1], 1);
    spwn->status = posix_spawnp(&spwn->child, head->cmd, &spwn->actions, NULL, head->av, environ);        
}

void mx_get_bytes_from_pipe(t_spawn_args *spwn, char **str, t_global *hd) {
    char *tmp = NULL;
    char *buf = NULL;

    if (spwn->status == 0) {
        spwn->status = waitpid(spwn->child, &spwn->status, 0);
        tmp = get_command_output(spwn, hd);
        if (*str == NULL) {
            *str = mx_strdup(tmp);
            free(tmp);
        }
        else {
            buf = mx_strjoin(*str, tmp);
            free(tmp);
            free(*str);
            *str = buf;
        }
    }
    else
        fprintf(stderr, "ush: %s\n", strerror(spwn->status));
    posix_spawn_file_actions_destroy(&spwn->actions);
}

char *mx_end_execution(char **str, t_lst *root, int cur_case) {
    mx_delete_list(root);
    if (cur_case == 1)
        return *str;
    return mx_strnew(1);
}
