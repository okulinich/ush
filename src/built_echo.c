#include "ush.h"

char *search_for_var(char *str) {
    extern char **environ;

    for(int i = 0; environ[i]; i++) {
        if(find_var_in_str(environ[i], str)) {
            return strdup(&environ[i][mx_strlen(str) + 1]);
        }
    }
    return NULL;
}

// \' одинарная кавычка
// \" двойная кавычка
// \? вопросительный знак
// \\ обратный слеш
// \0 нулевой символ
// \a звуковой сигнал
// \b забой
// \f перевод страницы - новая страница
// \n перевод строки - новая строка
// \r возврат каретки
// \t горизонтальная табуляция
// \v вертикальная табуляция

static void print_with_escape(char *str) {

    for(int i = 0; i < mx_strlen(str); i++) {
        if(str[i] == '\\' && (i + 1) < mx_strlen(str)
            && str[i + 1] != '\0') {
            switch(str[i + 1]) {
                case '\'':
                    mx_printchar('\'');
                    i++;
                    break;
                case '\"':
                    mx_printchar('\"');
                    i++;
                    break;
                case '?':
                    mx_printchar('\?');
                    i++;
                    break;
                case '\\':
                    mx_printchar('\\');
                    i++;
                    break;
                case '0':
                    mx_printchar('\0');
                    i++;
                    break;
                case 'a':
                    mx_printchar('\a');
                    i++;
                    break;
                case 'b':
                    mx_printchar('\b');
                    i++;
                    break;
                case 'f':
                    mx_printchar('\f');
                    i++;
                    break;
                case 'n':
                    mx_printchar('\n');
                    i++;
                    break;
                case 'r':
                    mx_printchar('\r');
                    i++;
                    break;
                case 't':
                    mx_printchar('\t');
                    i++;
                    break;
                case 'v':
                    mx_printchar('\v');
                    i++;
                    break;
                default:
                    mx_printchar(str[i]);
            }
        }
        else
            mx_printchar(str[i]);
    }
}

static void parse_flags(bool *no_new_line, bool *escape_off, t_lst *head, int *i) {
    bool flag = true;

    for(int j = 1; head->av[j]; j++) {
        if (head->av[j][0] == '-' && flag) {
            if (mx_strcmp(head->av[j], "-n") == 0 || mx_strcmp(head->av[j], "-enE") == 0 || mx_strcmp(head->av[j], "-Ene") == 0 || 
                mx_strcmp(head->av[j], "-nEe") == 0 || mx_strcmp(head->av[j], "-neE") == 0 ||
                mx_strcmp(head->av[j], "-Een") == 0 || mx_strcmp(head->av[j], "-eEn") == 0 || mx_strcmp(head->av[j], "-en") == 0) {
                *no_new_line = true;
                (*i)++;
            }
            else if(mx_strcmp(head->av[j], "-E") == 0) {
                *escape_off = true;
                (*i)++;
            }
            else if(mx_strcmp(head->av[j], "-e") == 0 || mx_strcmp(head->av[j], "-eE") == 0 || mx_strcmp(head->av[j], "-Ee") == 0) {
                *escape_off = false;
                (*i)++;
            }
            else if(mx_strcmp(head->av[j], "-nE") == 0 || mx_strcmp(head->av[j], "-En") == 0) {
                *no_new_line = true;
                *escape_off = true;
                (*i)++;
            }
            else if(mx_strcmp(head->av[j], "-ne") == 0 || mx_strcmp(head->av[j], "-en") == 0) {
                *no_new_line = true;
                *escape_off = false;
                (*i)++;
            }
        }
        else
            flag = false;
    }
}

int mx_echo(t_global *hd, t_lst *head) {
    char *str = NULL;
    bool no_new_line = false;
    bool escape_off = true;
    int i = 1;

    if(!head->av[1])
        mx_printstr("\n");
    else {
        parse_flags(&no_new_line, &escape_off, head, &i);
        for(; head->av[i]; i++) {
            if(mx_strcmp(head->av[i], "$?") == 0) {
                mx_printint(hd->last_exit_status);
            }
            else if(get_var_from_str(&head->av[i])) {
                while(get_var_from_str(&head->av[i]))
                    ;
            }
            if(mx_strcmp(head->av[i], "$?") != 0)
                escape_off == true ? mx_printstr(head->av[i]) : print_with_escape(head->av[i]);
            if(head->av[i + 1])
                mx_printstr(" ");
        }
        if(!no_new_line) {
            mx_printstr("\n");
        }
    }
    return 1;
}

char *get_variable_value(int pos, char *str) {
    int j = 0;
    bool ph_closed = false;
    char variable_name[256];
    char *variable_value = NULL;

    if(str[pos + 1] != '{') {
        while(isalpha(str[++pos])) {
            variable_name[j++] = str[pos];
        }
        ph_closed = true;
    }
    else {
        pos += 2;
        while(str[pos] != '}' && pos < mx_strlen(str))
            variable_name[j++] = str[pos++];
        if(pos < mx_strlen(str) && str[pos] == '}')
            ph_closed = true;
    }

    variable_name[j] = '\0';

    if(ph_closed)
        variable_value = search_for_var(variable_name);
    else
        variable_value = strdup("Error: odd number of parenthesis\n");
    return variable_value;
}

void replace_var_with_value(char **str, char *var_value, int i) {
    char *new_str = NULL;
    int g = 0;
    int k = i;

    if(var_value)
        new_str = mx_strnew(mx_strlen(*str) + mx_strlen(var_value));
    else
        new_str = mx_strnew(mx_strlen(*str));

    for(int j = 0; j < i; j++)
        new_str[g++] = (*str)[j];
    if(var_value) {
        for(int j = 0; j < mx_strlen(var_value); j++)
            new_str[g++] = var_value[j];
        free(var_value);
    }
    if((*str)[k + 1] != '{') 
        while(isalpha((*str)[++k]))
            ;
    else {
        while((*str)[k++] != '}')
            ;
    }
    while(k < mx_strlen(*str))
        new_str[g++] = (*str)[k++];
    
    free(*str);
    *str = new_str;
}


bool get_var_from_str(char **str) {
    bool var_found = false;
    int i;
    char *var_value;

    if(*str != NULL && mx_strlen(*str) > 1) {
        for(i = 0; i < mx_strlen(*str); i++)
            if((*str)[i] == '$' && i + 1 < mx_strlen(*str) 
                && (isalpha((*str)[i + 1]) || (*str)[i + 1] == '{')) {
                var_found = true;
                break;
            }
    }
    if(var_found) {
        var_value = get_variable_value(i, *str);
        if(var_value && var_value[0] == 'E' && var_value[1] == 'r' && var_value[2] == 'r') {
            free(*str);
            *str = var_value;
        }
        else {
            replace_var_with_value(str, var_value, i);
        }
    }
    return var_found;
}
