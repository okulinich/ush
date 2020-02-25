#ifndef USH_H
#define USH_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>
#include "../libmx/inc/libmx.h"

#define BUFSIZE 1024
#define DELIMITERS "\t\r\n\a "
#define COMMANDS 10
#define LOOP_BREAK 1
#define RETURN_EMPTY 2
#define LOOP_CONTINUE 3

// ctrl + z = 26
// ctrl + c = 3
// ctrl + d = 4
// up[0] = 27
// up[1] = 91
// up[2] = 65
// down[0] = 27
// down[1] = 91
// down[2] = 66
// left[0] = 27
// left[1] = 91
// left[2] = 68
// right[0] = 27
// right[1] = 91
// right[2] = 67

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


//зчитування, парсинг строки, формування списку команд для виконання
t_lst *lsh_read_line(t_cmd_history **hist);
//переводить термінал в неканонічний режим, зчитує строку, опрацьовує сигнали
char *noncanon_read_line(t_cmd_history **head);
//функція що перевіряє чи є строка командою
bool mx_is_command(char *str);
//функція що ділить строку на токени
char **ush_split_line(char *line);
//додавання елемента в кінець списку команд
t_lst *push_back(t_lst **head, char *command);
//додавання елемента в історію
void push_front_history(t_cmd_history **head, char *line);
//повертає масив команд підтримуваних інтерпретатором (для розпізнавання команд в строці)
char **get_commands();
//видалення списку команд і аргументів
void delete_list(t_lst *head);
//додає аргумент до поточної команди в списку
void add_new_arg(t_lst *tmp, char *arg);
//видаляє список що містить історію
void delete_history(t_cmd_history *head);
//опрацювання історії та backspace
bool history_or_backsp(char *ch, t_cmd_history **cur, char **line, int *i);
//опрацювання клавіш-стрілок праворуч і ліворуч
bool left_right_key(char ch[4], char **line, int *i);
//перевіряє чи була натиснута клавіша за її кодом
bool arrow_pressed(char *str, int a, int b, int c);
//заповнює строку '\0'
void mx_line_alloc(char *line);

#endif
