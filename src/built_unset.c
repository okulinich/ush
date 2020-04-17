#include "ush.h"

int mx_unset(t_global *hd, t_lst *head) {
    char *var_name = NULL;
    int index = 0;
    int j;

    for (int i = 1; head->av[i]; i++) {
        var_name = head->av[i];
        index = search_for_var_in_env(hd, var_name);
        if(index != -1) {
            free(hd->env[index]);
            for(j = index; hd->env[j + 1]; j++) {
                hd->env[j] = hd->env[j + 1];
            }
            hd->env[j] = NULL;
            hd->env = realloc(hd->env, sizeof(char *) * j + 1);
            var_name = mx_strndup(var_name, mx_get_char_index(var_name, '='));
            unsetenv(var_name);
            free(var_name);
        }
//meybe we will add variables, so let this code below be
/*        index = search_for_var_in_vars(hd, var_name);
        if(index != -1) {
            free(hd->vars[index]);
            for(j = index; hd->vars[j + 1]; j++) {
                hd->vars[j] = hd->vars[j + 1];
            }
            hd->vars[j] = NULL;
            hd->vars = realloc(hd->vars, sizeof(char *) * j + 1);
        }*/
    }

    return 1;
}
