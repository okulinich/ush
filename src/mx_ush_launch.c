#include "ush.h"

static bool match(char *src, char *regex) {
    regex_t reg;
    int result;

    regcomp(&reg, regex, REG_EXTENDED);
    result = regexec(&reg, src, 0, NULL, 0);
    regfree(&reg);
    return result == 0;
}

static int print_exec_error(t_lst *new) {
    if (!match(new->cmd, "/")) {
        fprintf(stderr, "ush: %s: Command not found\n", new->cmd);
        return 127;
    }
    else {
        for (int i = 0; new->av[i]; i++)
            if (match(new->av[i], "/"))
                fprintf(stderr, "ush: %s: No such file or directory\n", new->av[i]);
    }
    return 127;
}

void mx_display_error(t_lst *head) {
    char *cmd_name = NULL;

    mx_str_reverse(head->cmd);
    cmd_name = mx_strndup(head->cmd, mx_get_char_index(head->cmd, '/'));
    mx_str_reverse(cmd_name);
    fprintf(stderr, "env: %s: %s\n", cmd_name, strerror(errno));
    free(cmd_name);
}

static int func_exec(t_global *hd, t_lst *head) {
    int status = 0;
    char **new_env;

    if (mx_strcmp(head->cmd, "env") == 0) {
        hd->env = mx_env_copy();
        new_env = mx_parse_env_args(&hd); 
        mx_del_strarr(&hd->env);
        if (new_env == NULL) //інакше - повертає масив змінних среди
            return 1;
        else
            status = execve(head->cmd, head->av, new_env); 
        mx_display_error(head);
        mx_del_strarr(&new_env);
    }
    else
        status = execvp(head->cmd, head->av); 
    return status;
}

int mx_ush_launch(t_global *hd, t_lst *head) {
    pid_t pid;   
    pid_t wpid;  
    int status;
    int res = 0;

    pid = fork();
    if (pid == 0) {  
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        if ((res = func_exec(hd, head)) == 1)
            exit(1);
        if (res < 0) {
            print_exec_error(head);
            mx_strdel(&head->cmd);
            exit(127);
            return 127;
        }
    }
    wpid = waitpid(pid, &status, WUNTRACED);
    tcsetpgrp(0, getpid());
    hd->last_exit_status = WEXITSTATUS(status);
    return 1;
}
