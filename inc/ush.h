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
#define COMMANDS 10

typedef struct s_lst {
    char *cmd; //"ls"
    char **av; //{ "ls" , "-la", "src", null}
    struct s_lst *next;
} t_lst;

//перемкнути термінал в неканочнічний режим
void switch_noncanon(struct termios *savetty, struct termios *tty);
//повернути термінал в канонічний режим
void switch_canon(struct termios *savetty);
//зчитування, парсинг строки, формування списку команд для виконання
t_lst *lsh_read_line();
//функція що перевіряє чи є строка командою
bool mx_is_command(char *str);
//функція що ділить строку на токени
char **ush_split_line(char *line);
//додавання елемента в кінець списку команд
t_lst *push_back(t_lst **head, char *command);

char **get_commands();

void add_new_arg(t_lst *tmp, char *arg);


#endif
