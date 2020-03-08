#include "ush.h"

/* 
* -1 - not flagline
*/
static int get_flags_from_line(char *str) {
    int i = 0;
    int flags = 0;

    if (str[i + 1] == '\0') 
        return 4; // - flag
    while(str[++i]) {
        if (str[i] == 's') 
            flags |= 1;
        else if (str[i] == 'P') 
            flags |= 2;
        else {
            return -1;
        }
    }
    return flags;
}

/* 
* 1-st bit -> -s
* 2-nd bit -> -P
* 3-rd bit -> -
*/
static int get_flags(char **argv, int *i) {
    int flags = 0;

    while(argv[++(*i)]) {
        if (mx_strcmp(argv[(*i)], "--") == 0) {
            (*i)++;
            return flags;
        }
        if (argv[(*i)][0] != '-')
            return flags;
        int curr = get_flags_from_line(argv[*i]);
        if (curr == -1)
            return 0;
        flags |= curr;
    }
    i--;
    return flags;
}

int mx_chdir_p(char *path, char flags) {
    if (path == 0)
        return 0;
    setenv("OLDPWD", getenv("PWD"), 1);
    if (chdir(path) == -1) {
        if ((flags & 1) == 0)
            fprintf(stderr, "cd: %s: %s\n", strerror(errno), path);
        return 1;
    }
    setenv("PWD", getcwd(NULL, 0), 1);
    return 0;
}


static int cd(char **argv) {
    int i = 0;
    int flags = get_flags(argv, &i);
    char *path = (flags & 4) ? getenv("OLDPWD") : (argv[i] ? argv[i]
        : getenv("HOME"));
    int status = 0;

    if (mx_islink(path) && (flags & 1) && (flags & 2) == 0) {
        fprintf(stderr, "cd: not a directory: %s\n", argv[i]);
        return 1;
    }
    if (flags & 2)
        status = mx_chdir_p(path, flags);
    else
        status = mx_chdir_l(path, flags);
    return 1;
}

// static char **getav(char **av) {
//     char **res = NULL;
//     int j = 0;

//     if (!av)
//         return NULL;
//     res = (char **)malloc((mx_arr_size(av)) * sizeof(char *));
//     for (int i = 1; av[i]; i++, j++)
//         res[j] = strdup(av[i]);
//     res[j] = NULL;
//     return res;
// }

int	mx_cd(t_global *s, t_lst *h) {
    int status = cd(h->av);
    // int status = cd(h->av, s);
    // status = cd(h->av);
    mx_del_strarr(&s->env);
    // s->env = NULL;
    s->env = mx_env_copy();
    return status;
}
