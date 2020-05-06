#include "ush.h"

static int get_flags_from_line(char *str) {
    int i = 0;
    int flags = 0;

    if (str[i + 1] == '\0') 
        return 4;
    while(str[++i]) {
        if (str[i] == 's') 
            flags |= 1;
        else if (str[i] == 'P') 
            flags |= 2;
        else
            return -1;
    }
    return flags;
}

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

static int cd(char **argv, t_dirs *d, int flags, int i) {
    char *path = NULL;
    int status = 0;

    if (argv[i] && mx_strcmp(argv[i], ".") == 0)
        return 0;
    if (flags & 4)
        path = strdup(d->oldpwd); 
    else
        path = argv[i] ? strdup(argv[i]) : strdup(d->home);
    if (mx_islink(path) && (flags & 1) && (flags & 2) == 0) {
        fprintf(stderr, "cd: not a directory: %s\n", argv[i]);
        mx_strdel(&path);
        return -1;
    }
    status = (flags & 2 ? mx_cd_p(path, flags, d) : mx_cd_l(path, flags, d));
    mx_strdel(&path);
    return status;
}

static t_dirs *initpwd(void) {
    t_dirs *d = malloc(sizeof(t_dirs));

    d->pwd = getenv("PWD");
    d->oldpwd = getenv("OLDPWD");
    d->home = getenv("HOME");
    return d;
}

int	mx_cd(t_global *s, t_lst *h) {
    int status = 0;
    t_dirs *d = initpwd();
    int i = 0;
    int flags = get_flags(h->av, &i);
    
    if (flags & 4) {
        if (d->oldpwd == NULL) {
            mx_printerr("ush: cd: OLDPWD not set\n");
            s->last_exit_status = 1;
            return -1;
        }
        else
            printf("%s\n", d->oldpwd);
    }  
    if ((status = cd(h->av, d, flags, i)) == -1) {
        free(d);
        return -1;    
    }
    free(d);
    return status;
}
