#include "ush.h"

void mx_find_var_in_argument(int *i, char **str, bool *var_found) {
    for (*i = 0; *i < mx_strlen(*str); (*i)++) {
        if ((*str)[*i] == '$' && *i + 1 < mx_strlen(*str) 
            && (isalpha((*str)[*i + 1]) || (*str)[*i + 1] == '{')) {
            *var_found = true;
            break;
        }
    }
}

bool mx_get_var_from_str(char **str) {
    bool var_found = false;
    int i;
    char *var_value;

    if (*str != NULL && mx_strlen(*str) > 1)
        mx_find_var_in_argument(&i, str, &var_found);
    if (var_found) {
        var_value = mx_get_variable_value(i, *str);
        if (var_value && var_value[0] == 'E' && var_value[1] == 'r' 
            && var_value[2] == 'r') {
            free(*str);
            *str = var_value;
        }
        else
            mx_replace_var_with_valuev(str, var_value, i);
    }
    return var_found;
}

void mx_add_tokens_to_arr(char ***global, char ***temp, int *gl_i) {
    for (int j = 0; (*temp)[j] != NULL; j++)
        (*global)[(*gl_i)++] = mx_strdup((*temp)[j]);
    mx_del_strarr(temp);
}
