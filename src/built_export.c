#include "ush.h"

//якщо знаходить змінну повертає *=value* інакше = 0
static char *search_for_var(t_global *hd, char *str) {
    for(int i = 0; hd->vars[i]; i++) {
        if(find_var_in_str(hd->vars[i], str)) {
            return &hd->vars[i][mx_strlen(str)];
        }
    }
    for(int i = 0; hd->env[i]; i++) {
        if(find_var_in_str(hd->env[i], str)) {
            return &hd->env[i][mx_strlen(str)];
        }
    }
    return NULL;
}

//якщо знаходить змінну повертає *=value* інакше = 0
static char *search_for_var_in_env(t_global *hd, char *str) {
    for(int i = 0; hd->env[i]; i++) {
        if(find_var_in_str(hd->env[i], str)) {
            return &hd->env[i][mx_strlen(str)];
        }
    }
    return NULL;
}

static void add_var_to_env(char *name, char *value, t_global *hd) {
    char *new_var = mx_strjoin(name, value);
    char *name_for_search = mx_strjoin(name, "=");
    int i;

    if(search_for_var_in_env(hd, name)) {
        //mx_printstr("replace var\n");
        for(i = 0; strstr(hd->env[i], name_for_search) == NULL; i++) ;
        free(hd->env[i]);
        hd->env[i] = mx_strdup(new_var);
    }
    else {
        //mx_printstr("add new var\n");
        for(i = 0; strstr(hd->env[i], "_=") == NULL; i++) ;
        hd->env = realloc(hd->env, (i + 3) * sizeof(char *));
        hd->env[i + 1] = mx_strdup(hd->env[i]);
        hd->env[i + 2] = NULL;
        free(hd->env[i]);
        hd->env[i] = mx_strdup(new_var);
    }
    free(name_for_search);
    free(new_var);
}

int mx_export(t_global *hd, t_lst *head) {
    char *new_var = NULL;

    if(!head->av[1])
        return 1;
    else if(mx_get_char_index(head->av[1], '=') < 0) {  //якщо в строці немає =
        new_var = search_for_var(hd, head->av[1]);
        if(new_var) {                           //якщо змінну знайдено в масиві env або в масиві змінних->
            add_var_to_env(head->av[1], new_var, hd);   //експортуємо її в нове значення в env
        }
        else
            return 1;
    }
    else {                                              //якщо = є
        mx_printstr("not yet realized\n");
    }
    return 1;
}
