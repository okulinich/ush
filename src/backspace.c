#include "ush.h"

bool mx_arrow_pressed(char *str, int a, int b, int c) {
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

static void del_space(char **line, int *i, char *move_left, int prev_len) {
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
}

static void backspace(char ch, char **line, int *i) {
    char *move_left = mx_strjoin(" ", "[1D");
    int prev_len = strlen(*line);

    if(prev_len == 0) {
        free(move_left);
        return;
    }
    move_left[0] = ch;
    del_space(line, i, move_left, prev_len);
    mx_printstr(move_left);
    free(move_left);
}

bool mx_if_backsp(char *ch, char **line, int *i) {
    bool res = false;
    char esc = 27;

    if (ch[0] == 127) {                    //backspace
        if (*i > 0)
            backspace(esc, line, i);
        res = true;
    }
    return res;
}

