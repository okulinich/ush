#include "ush.h"

////////////////окремі функції для флагів ENV/////////////////////////
static char **i_flag_env(t_global **hd) {
    char **new_env;
    int i;
    char *folder_to_search = NULL;

    if (!(*hd)->new->av[2])             
        return NULL;
    else {
        new_env = mx_fill_new_env(hd, &i); 
        if (!mx_check_p_flag_args(hd, &folder_to_search, &i))
            return NULL;
        if ((*hd)->new->av[i]) {           
            mx_reparse_input_for_env(hd, i, folder_to_search);   
            return new_env;
        }
        else {
            mx_print_env(new_env);         
            mx_del_strarr(&new_env);
            return NULL;
        }
    }
}

static char **u_flag_env(t_global **hd) {
    char **new_env;

    if (!(*hd)->new->av[2]) {         
        mx_usage_env("u");
        return NULL;
    }
    else {
        new_env = mx_ignore_variables(hd); 
        if (!(*hd)->new->av[3]) {          
            mx_print_env(new_env);         
            return NULL;
        }
        else {
            mx_reparse_input_for_env(hd, 3, NULL);  
            return new_env;
        }
    }
}

static char **p_flag_env(t_global **hd) {
    char **new_env;
    char *folder_to_search = NULL;

    if (!(*hd)->new->av[2]) {         
        mx_usage_env("P");
        return NULL;
    }
    else {
        if (!(*hd)->new->av[3]) {
            mx_print_env((*hd)->env);
            return NULL;
        }
        else {
            new_env = mx_env_copy();
            folder_to_search = mx_strdup((*hd)->new->av[2]);
            mx_reparse_input_for_env(hd, 3, folder_to_search);
            return new_env;
        }
    }
}


char **mx_case_p_or_noflags(t_global **hd, char ***new_env) {
    if (mx_strcmp((*hd)->new->av[1], "-P") == 0){
        *new_env = p_flag_env(hd);
        if (*new_env != NULL)
            return *new_env;
        return NULL;
    }
    else {
        mx_reparse_input_for_env(hd, 1, NULL);
        *new_env = mx_env_copy();
        return *new_env;
    }
    return NULL;
}

/////////////          ОСНОВНИЙ АЛГОРИТМ env         /////////////
char **mx_parse_env_args(t_global **hd) {
    char **new_env;

    if (!(*hd)->new->av[1]) {         
        mx_print_env((*hd)->env);
        return NULL;
    }
    else if (mx_strcmp((*hd)->new->av[1], "-i") == 0) {
        new_env = i_flag_env(hd);
        if (new_env != NULL)
            return new_env;
        return NULL;
    }
    else if (mx_strcmp((*hd)->new->av[1], "-u") == 0) {
        new_env = u_flag_env(hd);
        if (new_env != NULL)
            return new_env;
        return NULL;
    }
    else 
        return mx_case_p_or_noflags(hd, &new_env);
}
