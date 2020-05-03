#include "ush.h"

int mx_unset(t_lst *head)  {
    char *var_name = NULL;
    int index = 0;
    int res = 1;

    for (int i = 1; head->av[i]; i++) {
        var_name = head->av[i];
        index = search_for_var_in_env(var_name);
        if(index != -1) {
            res = unsetenv(var_name);
            if(res == 0) {
                res = 1;
            }
            else {
                res = -1;
            }
        }
        else
            res = -1;
    }
    return res;
}
