#include "ush.h"

int mx_find_builtin(t_global *hd, t_lst *head) {
    if (mx_strcmp(head->cmd, "pwd") == 0)
        return mx_builtin_pwd(hd); // ret 1
    if (mx_strcmp(head->cmd, "exit") == 0)
        return mx_exit(hd); // ret 1
    if (mx_strcmp(head->cmd, "env") == 0)
        return mx_ush_launch(hd, head); // ret 1
    if(mx_strcmp(head->cmd, "echo") == 0)
        return mx_echo(hd, head);
    if(mx_strcmp(head->cmd, "export") == 0)
        return mx_export(hd, head);
    if(mx_strcmp(head->cmd, "cd") == 0) {
        mx_printstr("HELLO");
        mx_print_strarr(head->av, "\n");
        return 1;
    }
    
    return 0;
}

int mx_ush_execute(t_global *hd, t_lst *head) {
    if (head->cmd == NULL)
        return 1;
    return mx_find_builtin(hd, head) ? 1 : mx_ush_launch(hd, head);
    // если нашло билтин то рет 1 иначе лаунч
}
