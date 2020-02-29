#include "ush.h"

int mx_cd(t_lst *head) {
    if(!head->av[1]) {
        mx_printstr("No arg for cd\n");
    }
    else {
        if(chdir(head->av[1]) != 0)
            perror("u$h");
    }
    return 1;
}
