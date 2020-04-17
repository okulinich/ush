#include "ush.h"

static char *search_for_var(t_global *hd, char *str) {
    for(int i = 0; hd->env[i]; i++) {
        if(find_var_in_str(hd->env[i], &str[1])) {
            return &hd->env[i][mx_strlen(str)];
        }
    }
    for(int i = 0; hd->vars[i]; i++) {
        if(find_var_in_str(hd->vars[i], &str[1])) {
            return &hd->vars[i][mx_strlen(str)];
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
            if(head->av[i][0] == '$') {
                str = search_for_var(hd, head->av[i]);
                if(str != NULL) {
                    mx_printstr(str);
                }
            }
            else {
                escape_off == true ? mx_printstr(head->av[i]) : print_with_escape(head->av[i]);
            }
            if(head->av[i + 1])
                mx_printstr(" ");
        }
        if(!no_new_line) {
            mx_printstr("\n");
        }
    }
    return 1;
}