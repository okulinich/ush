#include "ush.h"

bool arrow_pressed(char *str, int a, int b, int c) {
    if(mx_strlen(str) == 3 && str[0] == a && str[1] == b && str[2] == c)
        return true;
    else
        return false;
}

static void rewr_str_after_space(int *i, char **line, char *move_left) {
    int j = *i;

    while(j < mx_strlen(*line)) {
        (*line)[j - 1] = (*line)[j];
        j++;
    }
    (*line)[mx_strlen(*line) - 1] = '\0';
    (*i)--;
    j = *i;
    mx_printstr(move_left);
    while (j < mx_strlen(*line))
        write(1, &(*line)[j++], 1);
    write(1, " ", 1);
    mx_printstr(move_left);
    while(j-- > *i + 1)
        mx_printstr(move_left);
}

void backspace(char ch, char **line, int *i) {
    char *move_left = mx_strjoin(" ", "[1D");
    int prev_len = strlen(*line);

    if(prev_len == 0) {
        free(move_left);
        return;
    }
    move_left[0] = ch;
    if((*line)[*i - 1] == '\t')
        for(int i = 0; i < 5; i++)
            mx_printstr(move_left);
    else {
        mx_printstr(move_left);
        mx_printchar(' ');
    }
    if(*i == prev_len) {
        (*line)[prev_len - 1] = '\0';
        (*i)--;
    }
    else
        rewr_str_after_space(i, line, move_left);
    mx_printstr(move_left);
    free(move_left);
}

// static void print_from_history(char *cmd, int *i, bool *res, char **line) {
//     char esc = 27;
//     char *move_right = mx_strjoin(" ", "[1C");
//     char *move_left = mx_strjoin(" ", "[1D");

//     move_left[0] = esc;
//     move_right[0] = 27;
//     while((*i) < mx_strlen(*line)) {
//         (*i)++;
//         mx_printstr(move_right);
//     }
//     while((*i)-- != 0) {
//         mx_printstr(move_left);
//         mx_printchar(' ');
//         mx_printstr(move_left);
//     }
//     mx_printstr(cmd);
//     mx_line_alloc(*line);
//     strcpy(*line, cmd);
//     *i = strlen(*line);
//     *res = true;
//     free(move_left);
//     free(move_right);
// }

static void print_backsp_and_rewrite_str(char **line, int *i, bool *res) {
    char esc = 27;

    if(*i > 0)
        backspace(esc, line, i);
    *res = true;
}

bool backsp(char *ch, char **line, int *i) {
    bool res = false;

    if (ch[0] == 127)                    //backspace
        print_backsp_and_rewrite_str(line, i, &res);
    return res;
}

