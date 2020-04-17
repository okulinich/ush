#include "ush.h"

static int get_slash(char *path) {
    int i = -1;
    int slash = 0;

    while(path[++i])
        if ((path[i] == '/') && (path[i + 1]))
            slash++;
    return slash;
}

static char *get_new_pwd(char *path, t_dirs *d) {
    char **tokens = mx_strsplit(path, '/');
    char *tmp_pwd = path[0] == '/' ? mx_strdup("/") : mx_strdup(d->pwd);
    char *res = 0;

    for (int i = 0; tokens[i]; i++)
        if (mx_strcmp(tokens[i], "..") == 0) {
            if (get_slash(tmp_pwd) == 0)
                continue;
            tmp_pwd = mx_del_last_slash(tmp_pwd);
        }
        else 
            tmp_pwd = mx_add_one_slash(tmp_pwd, tokens[i]);
    if (tmp_pwd[0] != '/') {
        res = mx_strjoin("/", tmp_pwd);
        free(tmp_pwd);
    }
    else
        res = tmp_pwd;
    mx_del_strarr(&tokens); 
    return res;
}

static char *pwd(char *path, t_dirs *d) {
    int len = mx_strlen(path);
    char *new_path = mx_strnew(len);
    char *ret = NULL;

    for (int i = 1, j = 0; i < len; i++, j++)
        new_path[j] = path[i];
    ret = mx_strjoin(d->home, new_path);
    mx_strdel(&new_path);
    return ret;
}

int mx_cd_l(char *path, char flags, t_dirs *d) {
    char *new_pwd = 0;

    if (path == 0)
        return 1;
    if (path[0] == '~' && path[1] == '/')
        new_pwd = pwd(path, d);
    else
        new_pwd = mx_strcmp(path, "~") == 0 ? strdup(d->home) : get_new_pwd(path, d);
    d->oldpwd = d->pwd;
    setenv("OLDPWD", d->pwd, 1);
    if (chdir(new_pwd) == -1) {
        if ((flags & 1) == 0)
        fprintf(stderr, "cd: %s: %s\n", strerror(errno), new_pwd);
        free(new_pwd);
        return 1;
    }
    d->pwd = new_pwd;
    setenv("PWD", d->pwd, 1);
    free(new_pwd);
    return 1;
}
