#include "ush.h"

char *mx_add_one_slash(char *path, char *new_part) {
    int len_path = mx_strlen(path);
    char *new_path = 0;

    if (path[len_path - 1] != '/')
        new_path = mx_strjoin(path, "/");
    new_path = mx_strjoin2(new_path, new_part);
    free(path);
    return new_path;
}

char *mx_del_last_slash(char *path) {
    char *new_path = 0;
    int len = mx_strlen(path);
    int i = len;

    while (path[--i] && path[i] != '/')
        path[i] = 0;
    if ((path[i] == '/') && (i != 0))
        path[i] = 0;
    new_path = mx_strdup(path);
    free(path);
    return new_path;
}
