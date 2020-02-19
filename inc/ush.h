#ifndef USH_H
#define USH_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>

#define BUFSIZE 1024
#define DELIMITERS "\t\r\n\a "
#define COMMANDS "export, unset, fg, exit, env, cd, pwd, which, echo"

typedef struct s_lst {
    char *cmd; //"ls"
    char **av; //{"-la", "src"}
    struct s_lst *next;
} t_lst;

void switch_noncanon(struct termios *savetty, struct termios *tty);
void switch_canon(struct termios *savetty);
char *lsh_read_line();
bool mx_is_command(char *str);
char **ush_split_line(char *line);


#endif
