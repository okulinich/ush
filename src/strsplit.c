#include "ush.h"

//кожну строку з av[] ділимо на підстроки по крапці з комою
//якщо перший аргумент - не команда, тоді шукаємо ; і відповідно нову команду
//інакше - записуємо перший аргумент як команду, а решту аргументів 
//(до найближчої крапки з комою) - як аргументи цієї команди
//має вийти наступна конструкція: {ls} {-a} {-l} {src} {;} {cd} {inc} {;} {pwd}
bool mx_string_has_chars(char *str) {
    bool res = false;

    for (int i = 0; i < mx_strlen(str); i++)
        if (!isspace(str[i]))
            res = true;
    return res;
}

char *bad_strdup(char *str, int i) {
    int j = 0;

    if (mx_get_char_index(str, ' ') < 0)
        return mx_strndup(str, i);
    else {
        j = mx_strlen(str) - 1;
        while (str[j] != ' ')
            j--;
        return mx_strndup(&str[j + 1], i - j - 1);
    }
}

void mx_add_semicol_to_av(char **str_arr, int *substr, char *str, int i) {
    str_arr[*substr] = bad_strdup(str, i);
    if (mx_strlen(str_arr[*substr]) == 0) {
        free(str_arr[*substr]);
        str_arr[*substr] = mx_strdup(";");
        (*substr)++;
    }
    else {
        (*substr)++;
        str_arr[*substr] = mx_strdup(";");
        (*substr)++;
    }
}

//функція повертає масив строк поділених по делімітеру у вигляді {ls} {;} {pwd} {NULL}
static char **split_to_substr(int *num_of_substr, char c, char *str) {
    char **str_arr = (char **)malloc(sizeof(char *) * 256);
    int substr = 0;

    str_arr[substr] = NULL;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == c) {
            str_arr[substr] = bad_strdup(str, i);
            mx_add_semicol_to_av(str_arr, &substr, str, i);
            if (mx_get_char_index(&str[i + 1], ';') < 0) {
                str_arr[substr] = mx_strdup(&str[i + 1]);
                substr++;
                break;
            }
            else
                str[i] = ' ';
        }
    }
    *num_of_substr = substr;
    str_arr[substr] = NULL;
    return str_arr;
}

void mx_split_by_delimiter(char ***av) {
    int num_of_substr = 0;
    char **substrings = NULL;
    int substr_indx = 0;

    for (int i = 0; (*av)[i]; i++) {
        if (mx_get_char_index((*av)[i], ';') >= 0 
            && mx_strcmp((*av)[i], ";") != 0) {
            substrings = split_to_substr(&num_of_substr, ';', (*av)[i]);
            if ((!substrings || substrings[0] != NULL))  {
                substr_indx =  mx_get_substring_index(*av, (*av)[i]);
                mx_replace_arg_with_arr(av, substr_indx, substrings);
            }
            mx_del_strarr(&substrings);
            substrings = NULL;
        }
        //якщо немає ; - пропускаємо 
    }
}

