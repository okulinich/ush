#include "ush.h"

int mx_find_builtin(t_global *hd) {
    if (mx_strcmp(hd->new->cmd, "pwd") == 0)
        return mx_builtin_pwd(hd->new); // ret 1
    if (mx_strcmp(hd->new->cmd, "exit") == 0)
        return mx_exit(hd->new); // ret 1
    if (mx_strcmp(hd->new->cmd, "env") == 0)
        return mx_ush_launch(hd); // ret 1
    return 0;
}

int mx_ush_execute(t_global *hd) {
    if (hd->new->cmd == NULL)
        return 1;
    return mx_find_builtin(hd) ? 1 : mx_ush_launch(hd);
    // если нашло билтин то рет 1 иначе лаунч
}
