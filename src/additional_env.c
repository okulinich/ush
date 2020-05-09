#include "ush.h"

//функція перевіряє чи менша строка входить в більшу(до = в більшій)
bool mx_find_var_in_str(char *big, char *little) {
    int i = 0;
    bool res = true;

    if (big[0] == little[0]) {
        while (big[i] != '\0' && big[i] != '=') {
            if (big[i] != little[i]) {
                res = false;
                break;
            }
            i++;
        }
        if (res)
            return res;
    }
    return false;
}

//функція для реалізації -u (ігнорує записані користувачем змінні)
char **mx_ignore_variables(t_global **hd) {
    int size = 0;
    char **new_env = NULL;
    int j = 0;

    while((*hd)->env[size])
        size++;
    new_env = (char **)malloc(sizeof(char *) * size + 1);
    for(int i = 0; i < size; i++) {
        if(!mx_find_var_in_str((*hd)->env[i], (*hd)->new->av[2]))
            new_env[j++] = mx_strdup((*hd)->env[i]);
    }
    new_env[j] = NULL;
    return new_env;
}

bool mx_check_p_flag_args(t_global **hd, char **folder_to_search, int *i) {
    if (mx_strcmp((*hd)->new->av[2], "-P") == 0) {
        if ((*hd)->new->av[3]) {
            if (!(*hd)->new->av[4])   
                return false;
            *folder_to_search = mx_strdup((*hd)->new->av[3]);
            *i = 4;
        }
        else {
            mx_usage_env("P");
            return false;
        }
    }
    return true;
}

