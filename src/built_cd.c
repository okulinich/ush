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

static int cd(char **argv, t_dirs *d) {
    int i = 0;
    int flags = get_flags(argv, &i);
    char *path = NULL;
    int status = 0;

    if (flags & 4)
        path = strdup(d->oldpwd);
    else
        path = argv[i] ? strdup(argv[i]) : strdup(d->home);
    if (mx_islink(path) && (flags & 1) && (flags & 2) == 0) {
        fprintf(stderr, "cd: not a directory: %s\n", argv[i]);
        mx_strdel(&path);
        return 1;
    }
    if (flags & 2)
        status = mx_cd_p(path, flags, d);
    else
        status = mx_cd_l(path, flags, d);
    mx_strdel(&path);
    return status;
}

static t_dirs *initpwd() {
    t_dirs *d = malloc(sizeof(t_dirs));

    d->pwd = getenv("PWD");
    d->oldpwd = getenv("OLDPWD");
    d->home = getenv("HOME");
    return d;
}

int	mx_cd(t_global *s, t_lst *h) {
    t_dirs *d = initpwd();
    int status = cd(h->av, d);
    
    mx_del_strarr(&s->env);
    s->env = mx_env_copy();
    free(d);
    // system("leaks -q ush");
    // exit(1);
    return status;
}
