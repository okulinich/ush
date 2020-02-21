#ifndef USH_H
#define USH_H

#include "libmx.h"

#define BUFSIZE 1024
#define DELIMITERS "\t\r\n\a "

typedef struct s_lst {
    char *cmd; //"ls"
    char **av; //{"-la", "src"}
    struct s_lst *next;
} t_lst;


#endif
