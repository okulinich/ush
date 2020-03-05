#include "ush.h"

static int flags_from_line(char *str) {
    int i = 0;
    int flags = 0;

    if (str[i + 1] == '\0')
        return 4; // - flag
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

static int i_flags(char **argv, int *i) {
   int flags = 0;

    while(argv[++(*i)]) {
        if (mx_strcmp(argv[(*i)], "--") == 0) {
            (*i)++;
            return flags;
        }
        if (argv[(*i)][0] != '-')
            return flags;
        int curr = flags_from_line(argv[*i]);
        if (curr == -1)
            return 0;
        flags |= curr;
    }
    i--;
    return flags;
}
/*
    -1 = no flags from line
    - = 4 bit;
    -- = 0 bit;
    -s = 1 bit;
    -P = 2 bit;
*/
/////////// flags !!!

int	mx_cd(t_global *s, t_lst *h) {
    int i = 0;
    char flags = i_flags(h->av, &i);

                mx_printint(flags);
                if (h->av[i])
                    mx_printstr(h->av[i]);
        if (flags == 0) {
            mx_cd_sflags(h->av[i], s->env);
            // system("leaks -q ush");
            // exit (1);
        }
        else {
            mx_cd_lflags(h->av[i], s->env, flags);
        }
        // else {
            // if (mx_is_link(h->av[1]) && (flags & 1) && (flags & 2) == 0) {
                // fprintf(stderr, "cd: not a directory: %s\n", h->av[1]);
                // return 1; // for -P -s !!!!!!!!!!!!!!!!!!!!!!!
                // mx_cd_flags
        // }
    // }
    // system("leaks -q ush");
    // exit (1);
	return 1;
}
