#include "ush.h"

static int one_more_cycle(char **av, int i, int *p) {
    for (int j = 1; av[i][j]; j++) {
        if (av[i][j] == 'P')
            *p = 1;
        else if (av[i][j] != 'L' && 
                (av[i][j] != '-' || (av[i][j] == '-' && j == 2))) {
            fprintf(stderr, "pwd: bad option: -%c\n", av[i][j]);
            return -1;
        }
    }
    return 0;
}

static int get_flags(char **av) {
    bool stop = 0;
    int p = 0;

    for (int i = 1; av[i] != NULL; i++) {
        if (av[i][0] == '-' && !stop) {
            if ((av[i][1] == '-' && av[i][2] == '\0') || av[i][1] == '\0')
                stop = 1;
            if (one_more_cycle(av, i, &p) == -1)
                return -1;
        }
        else {
            fprintf(stderr, "pwd: too many arguments\n");
            return -1;
        }
    }
    return p;
}

static void pwd_default(void) {
    char *res = strdup(getenv("PWD"));

    printf("%s\n", res);
    mx_strdel(&res);
}

static void pwd_flagp(void) {
    char *path = strdup(getenv("PWD"));
    char realbuf[PATH_MAX];

    if (mx_islink(path)) {
        char *res = realpath(path, realbuf);
        if (res)
            printf("%s\n", realbuf);
    }
    else
        printf("%s\n", path);
    mx_strdel(&path);
}

int mx_builtin_pwd(t_lst *head) { 
    int p;

    if ((p = get_flags(head->av)) != -1) {
        if (p)
            pwd_flagp();
        else
            pwd_default();
    }
    return 1;
}
