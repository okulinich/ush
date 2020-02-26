#include "ush.h"

int mx_ush_execute(t_global *hd) {
    if(hd->new->cmd == NULL)
        return 1;
    if (mx_strcmp(hd->new->cmd, "pwd") == 0)
        return mx_builtin_pwd(hd->new);
    if (mx_strcmp(hd->new->cmd, "exit") == 0)
        mx_exit(hd->new);
        // функція яка знаходить білтін і юзає його
        // якщо енв то заходить ush_launch 
    return mx_ush_launch(hd);
}
