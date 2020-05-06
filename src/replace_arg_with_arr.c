#include "ush.h"

//saves least string after semicolon
int mx_save_least_str(char ***av, int indx, char ***least) {
    int least_size = 0;

    if ((*av)[indx + 1])
        for (int i = indx + 1; (*av)[i] != NULL; i++) 
            if (strlen((*av)[i]) > 0)
                least_size++;
    if (least_size > 0) {
        *least = (char **)malloc(sizeof(char *) * least_size + 1);
        for (int i = 0; i < least_size; i++)
            (*least)[i] = mx_strdup((*av)[indx + 1 + i]);
        (*least)[least_size] = NULL;
    }
    else
        least_size = 0;
    return least_size;
}

void mx_del_temp_arr_of_strings(int least_size, char ***av, char ***least) {
    mx_del_strarr(av);
    if (least_size > 0)
        mx_del_strarr(least);
}

//replaces string from av[] with arr of strings if it includes ';'
void mx_replace_arg_with_arr(char ***av, int indx, char **str_arr) {
    int least_size = 0;
    char **least;
    char **new_av = NULL;
    int str_arr_size;

    least_size = mx_save_least_str(av, indx, &least);
    for (str_arr_size = 0; str_arr[str_arr_size]; str_arr_size++) ;
    new_av = (char **)malloc(sizeof(char *) * (str_arr_size + least_size + indx + 1));
    for (int i = 0; i < indx; i++)
        new_av[i] = mx_strdup((*av)[i]);
    for (int i = 0; i < str_arr_size; i++)
        new_av[indx + i] = mx_strdup(str_arr[i]);
    if (least_size > 0)
        for (int i = 0; i < least_size; i++)
            new_av[str_arr_size + indx + i] = mx_strdup(least[i]);
    new_av[str_arr_size + least_size + indx] = NULL;
    mx_del_temp_arr_of_strings(least_size, av, &least);
    *av = new_av;
}

int mx_get_substring_index(char **arr, char *str) {
    for (int i = 0; arr[i]; i++)
        if (mx_strcmp(arr[i], str) == 0)
            return i;
    return -1;
}
