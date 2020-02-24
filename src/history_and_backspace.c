#include "ush.h"

bool arrow_pressed(char *str, int a, int b, int c) {
    if(strlen(str) == 3 && str[0] == a && str[1] == b && str[2] == c)
        return true;
    else
        return false;
}

void backspace(char ch, char **line) {
    char *move_left = mx_strjoin(" ", "[1D");

    if(strlen(*line) == 0) {
        free(move_left);
        return;
    }
    move_left[0] = ch;
    mx_printstr(move_left);
    mx_printchar(' ');
    (*line)[strlen(*line) - 1] = '\0';
    mx_printstr(move_left);

    free(move_left);
}

static void print_from_history(char *cmd, int *i, bool *res, char **line) {
    char esc = 27;

    while(strlen(*line) != 0)
        backspace(esc, line);
    mx_printstr(cmd);
    strcpy(*line, cmd);
    *i = strlen(*line);
    *res = true;
}

bool history_or_backsp(char *ch, t_cmd_history **cur, char **line, int *i) {
    bool res = false;
    char esc = 27;

    if (arrow_pressed(ch, 27, 91, 65)) {         //перевіряє чи натиснута клавіша вгору(?)
        if(*cur) {
            print_from_history((*cur)->user_input, i, &res, line);
            if((*cur)->next)
                *cur = (*cur)->next;
        }
    }
    else if (arrow_pressed(ch, 27, 91, 66)) {
        if(*cur && (*cur)->prev) {
            print_from_history((*cur)->prev->user_input, i, &res, line);
            *cur = (*cur)->prev;
        }
        else if(!(*cur)->prev) {
            print_from_history("", i, &res, line);
        }
    }
    else if (ch[0] == 127) {                    //backspace
        backspace(esc, line);
        (*i) = strlen(*line);
        res = true;
    }
    return res;
}

