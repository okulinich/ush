#include "ush.h"

//якщо знаходить змінну повертає *=value* інакше = 0
char *mx_ssearch_for_var_in_env(char *str) {
    extern char **environ;

    for(int i = 0; environ[i]; i++) {
        if(mx_find_var_in_str(environ[i], str)) {
            return mx_strdup(&environ[i][mx_strlen(str)]);
        }
    }
    return NULL;
}


void mx_add_str_to_env(char *str) {
    char *var_name = mx_strndup(str, mx_get_char_index(str, '='));
    char *value = mx_strdup(&str[mx_get_char_index(str, '=') + 1]);

    setenv(var_name, value, 1);
    free(var_name);
    free(value);    
}

int mx_export(t_lst *head) {
    for (int i = 1; head->av[i]; i++) {
        if (!head->av[i])
            return 1;
        else if (mx_get_char_index(head->av[i], '=') < 0) { 
            return -1;
        }
        else {
            mx_add_str_to_env(head->av[i]);
        }
    }
    return 1;
}
