#include "ush.h"

// //якщо знаходить змінну повертає *=value* інакше = 0
// static char *search_for_var(t_global *hd, char *str) {
//     for(int i = 0; hd->vars[i]; i++) {
//         if(find_var_in_str(hd->vars[i], str)) {
//             return &hd->vars[i][mx_strlen(str)];
//         }
//     }
//     for(int i = 0; hd->env[i]; i++) {
//         if(find_var_in_str(hd->env[i], str)) {
//             return &hd->env[i][mx_strlen(str)];
//         }
//     }
//     return NULL;
// }

//якщо знаходить змінну повертає *=value* інакше = 0
char *ssearch_for_var_in_env(char *str) {
    extern char **environ;

    for(int i = 0; environ[i]; i++) {
        if(find_var_in_str(environ[i], str)) {
            return mx_strdup(&environ[i][mx_strlen(str)]);
        }
    }
    return NULL;
}

// void add_var_to_env(char *name, char *value, t_global *hd) {
//     char *new_var = mx_strjoin(name, value);
//     char *name_for_search = mx_strjoin(name, "=");
//     int i;
//     char *temp = NULL;

//     temp = ssearch_for_var_in_env(hd, name);
//     if(temp) {
//         //mx_printstr("replace var\n");
//         // for(i = 0; strstr(hd->env[i], name_for_search) == NULL; i++) ;
//         // free(hd->env[i]);
//         // hd->env[i] = mx_strdup(new_var);
//         setenv(name, value, 1);
//     }
//     else {
//         //mx_printstr("add new var\n");
//         // for(i = 0; strstr(hd->env[i], "_=") == NULL; i++) ;
//         // hd->env = realloc(hd->env, (i + 3) * sizeof(char *));
//         // hd->env[i + 1] = mx_strdup(hd->env[i]);
//         // hd->env[i + 2] = NULL;
//         // free(hd->env[i]);
//         // hd->env[i] = mx_strdup(new_var);
//         setenv(name, value, 1);
//     }
//     if(temp)
//         free(temp);
//     free(name_for_search);
//     free(new_var);
// }

static void add_str_to_env(char *str) {
    char *var_name = mx_strndup(str, mx_get_char_index(str, '='));
    char *value = mx_strdup(&str[mx_get_char_index(str, '=') + 1]);

    setenv(var_name, value, 1);
    free(var_name);
    free(value);    
}

int mx_export(t_lst *head) {
    for(int i = 1; head->av[i]; i++) {
        if(!head->av[i])
            return 1;
        else if(mx_get_char_index(head->av[i], '=') < 0)  //якщо в строці немає =
            return -1;
        else                                              //якщо = є
            add_str_to_env(head->av[i]);
    }
    return 1;
}
