#include "ush.h"

static int get_rank(char *path) {
    int i = -1;
    int rank = 0;

    while(path[++i]) {
        if ((path[i] == '/') && (path[i + 1])) {
            rank++;
        }
    }
    return rank;
}

char *mx_add_one_rank(char *path, char *new_part) {
    int len_path = mx_strlen(path);
    char *new_path = 0;

    if (path[len_path - 1] != '/')
        new_path = mx_strjoin(path, "/");
    new_path = mx_strjoin2(new_path, new_part);
    free(path);
    return new_path;
}

char* mx_del_last_rank(char *path) {
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

static char *get_new_pwd(char *path) {
    char **tokens = mx_strsplit(path, '/');
    char *tmp_pwd = path[0] == '/' ? mx_strdup("/") : mx_strdup(getenv("PWD"));
    char *res = 0;

    for (int i = 0; tokens[i]; i++)
        if (mx_strcmp(tokens[i], "..") == 0) {
            if (get_rank(tmp_pwd) == 0)
                continue;
            tmp_pwd = mx_del_last_rank(tmp_pwd);
        }
        else 
            tmp_pwd = mx_add_one_rank(tmp_pwd, tokens[i]);
    if (tmp_pwd[0] != '/') {
        res = mx_strjoin("/", tmp_pwd);
        free(tmp_pwd);
    }
    else
        res = tmp_pwd;
    mx_del_strarr(&tokens);
    return res;
}

int mx_chdir_l(char *path, char flags) {
    char *new_pwd = 0;

    if (path == 0)
        return 0;
    if (mx_strcmp(path, "~") == 0)
        new_pwd = getenv("HOME");
    else 
        new_pwd = get_new_pwd(path);
    setenv("OLDPWD", getenv("PWD"), 1);
    if (chdir(new_pwd) == -1) {
        if ((flags & 1) == 0)
        fprintf(stderr, "cd: %s: %s\n", strerror(errno), new_pwd);
        free(new_pwd);
        return 1;
    }
    setenv("PWD", new_pwd, 1);
    return 1;
}
