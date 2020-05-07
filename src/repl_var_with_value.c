#include "ush.h"

char *search_for_var(char *str) {
    extern char **environ;

    for (int i = 0; environ[i]; i++) {
        if (find_var_in_str(environ[i], str)) {
            return mx_strdup(&environ[i][mx_strlen(str) + 1]);
        }
    }
    return NULL;
}


void mx_get_value_by_name(bool ph_closed, char **variable_value, 
                          char *variable_name, int j) {
    variable_name[j] = '\0';
    if (ph_closed)
        *variable_value = search_for_var(variable_name);
    else
        *variable_value = mx_strdup("Error: odd number of parenthesis\n");
}

char *get_variable_value(int pos, char *str) {
    int j = 0;
    bool ph_closed = false;
    char variable_name[256];
    char *variable_value = NULL;

    if (str[pos + 1] != '{') {
        while (isalpha(str[++pos])) {
            variable_name[j++] = str[pos];
        }
        ph_closed = true;
    }
    else {
        pos += 2;
        while (str[pos] != '}' && pos < mx_strlen(str))
            variable_name[j++] = str[pos++];
        if (pos < mx_strlen(str) && str[pos] == '}')
            ph_closed = true;
    }
    mx_get_value_by_name(ph_closed, &variable_value, variable_name, j);
    return variable_value;
}

void mx_write_value_to_str(char **str, int k, int g, char **new_str) {
    if ((*str)[k + 1] != '{') 
        while(isalpha((*str)[++k]))
            ;
    else
        while((*str)[k++] != '}')
            ;
    while (k < mx_strlen(*str))
        (*new_str)[g++] = (*str)[k++];
}

void replace_var_with_value(char **str, char *var_value, int i) {
    char *new_str = NULL;
    int g = 0;
    int k = i;

    if (var_value)
        new_str = mx_strnew(mx_strlen(*str) + mx_strlen(var_value));
    else
        new_str = mx_strnew(mx_strlen(*str));
    for (int j = 0; j < i; j++)
        new_str[g++] = (*str)[j];
    if (var_value) {
        for(int j = 0; j < mx_strlen(var_value); j++)
            new_str[g++] = var_value[j];
        free(var_value);
    }
    mx_write_value_to_str(str, k, g, &new_str);
    free(*str);
    *str = new_str;
}
