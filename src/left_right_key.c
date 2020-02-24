#include "ush.h"

//%c[1D", 27 - left

bool left_right_key(char ch[4], char **line, int *i) {
    if(arrow_pressed(ch, 27, 91, 68)) {
        if(strlen(*line) >= 1 && *i > 0) {
            mx_printstr(ch);
            (*i)--;
        }
        return true;
    }
    else if(arrow_pressed(ch, 27, 91, 67)) {
        if(*i < strlen(*line)) {
            mx_printstr(ch);
            (*i)++;
        }
        return true;
    }
    return false;
}
