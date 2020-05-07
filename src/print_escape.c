#include "ush.h"

static void print_with_escape_least(char *str, int *i) {
    switch(str[*i + 1]) {
        case 'n':
            mx_printchar('\n');
            (*i)++;
            break;
        case 'r':
            mx_printchar('\r');
            (*i)++;
            break;
        case 't':
            mx_printchar('\t');
            (*i)++;
            break;
        case 'v':
            mx_printchar('\v');
            (*i)++;
            break;
        default:
            mx_printchar(str[*i]);
    }
}

static void print_with_escape_second(char *str, int *i) {
    switch(str[*i + 1]) {
        case '0':
            mx_printchar('\0');
            (*i)++;
            break;
        case 'a':
            mx_printchar('\a');
            (*i)++;
            break;
        case 'b':
            mx_printchar('\b');
            (*i)++;
            break;
        case 'f':
            mx_printchar('\f');
            (*i)++;
            break;
        default:
            print_with_escape_least(str, i);
    }
}

static void print_with_escape_first_part(char *str, int *i) {
    switch(str[*i + 1]) {
        case '\"':
            mx_printchar('\"');
            (*i)++;
            break;
        case '?':
            mx_printchar('\?');
            (*i)++;
            break;
        case '\\':
            mx_printchar('\\');
            (*i)++;
            break;
        default:
            print_with_escape_second(str, i);
    }
}

void print_with_escape(char *str) {
    for (int i = 0; i < mx_strlen(str); i++) {
        if (str[i] == '\\' && (i + 1) < mx_strlen(str)
            && str[i + 1] != '\0') {
            switch(str[i + 1]) {
                case '\'':
                    mx_printchar('\'');
                    i++;
                    break;
                default:
                    print_with_escape_first_part(str, &i);
            }
        }
        else
            mx_printchar(str[i]);
    }
}

