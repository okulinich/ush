#include "ush.h"

void mx_line_alloc(char *line) {
    for (int i = 0; i < MX_BUFSIZE; i++)
        line[i] = '\0';
}

bool mx_left_right_key(char ch[4], char **line, int *i) {
    if (mx_arrow_pressed(ch, 27, 91, 68)) {
        if (mx_strlen(*line) >= 1 && *i > 0) {
            mx_printstr(ch);
            (*i)--;
        }
        return true;
    }
    else if (mx_arrow_pressed(ch, 27, 91, 67)) {
        if (*i < mx_strlen(*line)) {
            mx_printstr(ch);
            (*i)++;
        }
        return true;
    }
    return false;
}
