#ifndef USH_H
#define USH_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>
#include "./libmx/inc/libmx.h"

#define BUFSIZE 1024
#define DELIMITERS "\t\r\n\a "
#define COMMANDS 10



// ctrl + z = 26
// ctrl + c = 3
// ctrl + d = 4
// up[0] = 27
// up[1] = 91
// up[2] = 65
// down[0] = 27
// down[1] = 91
// down[2] = 66



typedef struct s_lst {
    char *cmd; //"ls"
    char **av; //{ "ls" , "-la", "src", null}
    struct s_lst *next;
} t_lst;

typedef struct s_cmd_history {
    char *user_input;
    struct s_cmd_history *next;
    struct s_cmd_history *prev;
} t_cmd_history;

//перемкнути термінал в неканочнічний режим
void switch_noncanon(struct termios *savetty, struct termios *tty);
//повернути термінал в канонічний режим
void switch_canon(struct termios *savetty);
//зчитування, парсинг строки, формування списку команд для виконання
t_lst *lsh_read_line(t_cmd_history **head);
//функція що перевіряє чи є строка командою
bool mx_is_command(char *str);
//функція що ділить строку на токени
char **ush_split_line(char *line);
//додавання елемента в кінець списку команд
t_lst *push_back(t_lst **head, char *command);
// повертає масив із уменами команд
char **get_commands();
//зчитує строку та сигнали з термінала
char *noncanon_read_line(t_cmd_history **head);
//дописує аргумент в список до команди
void add_new_arg(t_lst *tmp, char *arg);
//додає елемент до історії
void push_front_history(t_cmd_history **head, char *line);
//виводить попередній та наступний записи з історії
void print_next_cmd(t_cmd_history **cur, char **line);
void print_prev_cmd(t_cmd_history **cur, char **line);
//виконує функції історії та backspace
bool hist_or_backsp(char *ch, t_cmd_history **cur, char **line, int *i);
//
void backspace(char ch, char **line);
//перевіряє чи натиснута клавіша стрілки
bool arrow_pressed(char *str, int a, int b, int c);

#endif
