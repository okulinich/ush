#include "ush.h"


int mx_cd_p(char *path, char flags, t_dirs *d) {
    if (path == 0)
        return 1;
    d->oldpwd = d->pwd;
    setenv("OLDPWD", d->oldpwd, 1);
    if (chdir(path) == -1) {
        if ((flags & 1) == 0)
            fprintf(stderr, "cd: %s: %s\n", strerror(errno), path);
        return -1;
    }
    d->pwd = getcwd(NULL, 0);
    setenv("PWD", d->pwd, 1);
    free(d->pwd);
    return 1;
}
