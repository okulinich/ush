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
char *ssearch_for_var_in_env(t_global *hd, char *str) {
    for(int i = 0; hd->env[i]; i++) {
        if(find_var_in_str(hd->env[i], str)) {
            return &hd->env[i][mx_strlen(str)];
        }
    }
    return NULL;
}

void add_var_to_env(char *name, char *value, t_global *hd) {
    char *new_var = mx_strjoin(name, value);
    char *name_for_search = mx_strjoin(name, "=");
    int i;

    if(ssearch_for_var_in_env(hd, name)) {
        //mx_printstr("replace var\n");
        for(i = 0; strstr(hd->env[i], name_for_search) == NULL; i++) ;
        free(hd->env[i]);
        hd->env[i] = mx_strdup(new_var);
        setenv(name, value, 1);
    }
    else {
        //mx_printstr("add new var\n");
        for(i = 0; strstr(hd->env[i], "_=") == NULL; i++) ;
        hd->env = realloc(hd->env, (i + 3) * sizeof(char *));
        hd->env[i + 1] = mx_strdup(hd->env[i]);
        hd->env[i + 2] = NULL;
        free(hd->env[i]);
        hd->env[i] = mx_strdup(new_var);
        setenv(name, value, 1);
    }
    free(name_for_search);
    free(new_var);
}

static void add_str_to_env(t_global *hd, t_lst *head, int j) {
    int i = 0;
    char *var_name = mx_strndup(head->av[j], mx_get_char_index(head->av[j], '='));

    i = search_for_var_in_env(hd, var_name);
    if(i != -1) {
        free(hd->vars[i]);
        hd->vars[i] = mx_strdup(head->av[j]);
    }
    else {
        i = search_for_var_in_env(hd, "_=");
        hd->env = realloc(hd->env, (i + 3) * sizeof(char *));
        hd->env[i + 1] = mx_strdup(hd->env[i]);
        hd->env[i + 2] = NULL;
        free(hd->env[i]);
        hd->env[i] = mx_strdup(head->av[j]);
    }
    free(var_name);
}

int mx_export(t_global *hd, t_lst *head) {
    char *new_var = NULL;

    for(int i = 1; head->av[i]; i++) {
        if(!head->av[i])
            return 1;
        else if(mx_get_char_index(head->av[i], '=') < 0) {  //якщо в строці немає =
            new_var = ssearch_for_var_in_env(hd, head->av[i]);
            if(new_var) {                           //якщо змінну знайдено в масиві env або в масиві змінних->
                add_var_to_env(head->av[i], new_var, hd);   //експортуємо її в нове значення в env
            }
            else
                return -1;
        }
        else {                                              //якщо = є
            add_str_to_env(hd, head, i);
        }
    }
    return 1;
}
