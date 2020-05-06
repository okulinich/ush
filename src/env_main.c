#include "ush.h"

////////////////окремі функції для флагів ENV/////////////////////////
char **i_flag_env(t_global **hd) {
    char **new_env;
    int i;
    char *folder_to_search = NULL;

    if (!(*hd)->new->av[2])                  //якщо немає аргументів після 'і' тоді нічого не робимо 
        return NULL;
    else {
        new_env = fill_new_env(hd, &i);     //заповнюємо новий масив із введених змінних
        if (!mx_check_p_flag_args(hd, &folder_to_search, &i))
            return NULL;
        if ((*hd)->new->av[i]) {             //якщо далі є команда яку потрібно виконати тоді 
            reparse_input_for_env(hd, i, folder_to_search);   //передаємо їй новий масив і виконуємо цю команду в execve()
            return new_env;
        }
        else {                              //якщо команди немає, тоді просто виводимо
            mx_print_env(new_env);          //введені користувачем змінні
            mx_del_strarr(&new_env);
            return NULL;
        }
    }
}

char **u_flag_env(t_global **hd) {
    char **new_env;

    if (!(*hd)->new->av[2]) {         //якщо немає аргументів після 'u' тоді виводимо usage 
        mx_usage_env("u");
        return NULL;
    }
    else {
        new_env = ignore_variables(hd);    //копіюємо масив змінних оболонки
                                                // i видаляємо введені користувачем зміннні
        if (!(*hd)->new->av[3]) {            //якщо немає програми для запуску, тоді просто виводимо поточні змінні
            mx_print_env(new_env);          //ігноруючи задану користувачем
            return NULL;
        }
        else {
            reparse_input_for_env(hd, 3, NULL);       //якщо є програма для запуску, передаємо їй змінні, виключаючи задану користувачем
            return new_env;
        }
    }
}

char **p_flag_env(t_global **hd) {
    char **new_env;
    char *folder_to_search = NULL;

    if (!(*hd)->new->av[2]) {         //якщо немає аргументів після '-P' тоді виводимо usage 
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
            reparse_input_for_env(hd, 3, folder_to_search);
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
        reparse_input_for_env(hd, 1, NULL);
        *new_env = mx_env_copy();
        return *new_env;
    }
    return NULL;
}

/////////////          ОСНОВНИЙ АЛГОРИТМ env         /////////////
char **mx_parse_env_args(t_global **hd) {
    char **new_env;

    if (!(*hd)->new->av[1]) {          //якщо env без флагів тоді просто виводимо змінні оболонки
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
