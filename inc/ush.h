#ifndef USH_H
#define USH_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 1024
#define DELIMITERS "\t\r\n\a "

typedef struct s_lst {
    char *cmd; //"ls"
    char **av; //{"-la", "src"}
    struct s_lst *next;
} t_lst;

#endif
