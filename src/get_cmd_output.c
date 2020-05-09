#include "ush.h"


void mx_continue_execution(t_spawn_args *spwn, t_lst **head, 
                            char **str, t_global *hd) {
    mx_create_new_proccess(spwn, *head);
    mx_get_bytes_from_pipe(spwn, str, hd);
    *head = (*head)->next;
}

static char *recursive_call(char **str, t_lst *head, t_lst *root, t_global *hd) {
    int indx = mx_check_cmd_args_for_commands(head);

    *str = mx_get_cmd_output(head->av[indx], hd);
    return mx_end_execution(str, root, 1);
}

static void init_new_cmd_list(char *cmd, t_lst **head, t_lst **root) {
    *head = mx_parse_cmd_to_list(cmd);
    *root = *head;
}

static char *check_second_arg(t_lst *head, t_lst *root, char **str) {
    if (head->av[1] && mx_strcmp(head->av[1], "-n") == 0 
        && head->av[2])
        return mx_copy_echo_arg(head, root, 2);
    else
        return mx_end_execution(str, root, 2);
}

char *mx_get_cmd_output(char *cmd, t_global *hd) {
	t_spawn_args spwn;
    char *str = NULL;
    t_lst *head = NULL;
    t_lst *root = NULL;
    int indx = 0;

    init_new_cmd_list(cmd, &head, &root);
    while(head) {
        if (mx_strcmp(head->cmd, "echo") == 0) {
            indx = mx_check_cmd_args_for_commands(head);
            if (indx > 0)
                return recursive_call(&str, head, root, hd);
            if (head->av[1] && mx_strcmp(head->av[1], "-n") != 0)
                return mx_copy_echo_arg(head, root, 1);
            else
                return check_second_arg(head, root, &str);
        }
        mx_continue_execution(&spwn, &head, &str, hd);
    }
    return mx_end_execution(&str, root, 1);
}

