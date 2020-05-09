#include "ush.h"

void mx_escape_seq_and_split_by_semicol(char *line, char ***av) {
    if (*av)
        mx_del_strarr(av);
    *av = mx_ush_split_line(line, NULL);     
    mx_split_by_delimiter(av);               
}
