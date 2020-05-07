#include "ush.h"


int mx_cd_p(char *path, char flags, t_dirs *d) {
    if (path == 0)
        return 1;
    d->oldpwd = d->pwd;
    setenv("OLDPWD", d->oldpwd, 1);
    if (chdir(path) == -1) {
        if ((flags & 1) == 0)
            fprintf(stderr, "cd: %s: %s\n", strerror(errno), path);
        return 1;
    }
    d->pwd = getcwd(NULL, 0);
    setenv("PWD", d->pwd, 1);
    free(d->pwd);
    return 1;
}

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
