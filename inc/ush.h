#ifndef USH_H
#define USH_H

#include "libmx.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <regex.h>
#include <limits.h>
#include <sys/stat.h>

#define BUFSIZE 1024
#define DELIMITERS "\t\r\n\a "
#define COMMANDS 10
#define MX_RIGHT_ARROW "\x1b\x5b\x43"
#define MX_LEFT_ARROW "\x1b\x5b\x44"
#define MX_HOME_KEY "\x1b\x5b\x48"
#define MX_END_KEY "\x1b\x5b\x46"
#define LOOP_BREAK 1
#define RETURN_EMPTY 2
#define LOOP_CONTINUE 3
#define MX_WHITE "\033[30;48;5;231m"
#define MX_NORM "\033[0m"

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

typedef struct s_global {
    t_lst *new;
    char **env;
    char **vars;
} t_global;


// core
void mx_ush_loop(t_global *hd);
int mx_ush_execute(t_global *hd, t_lst *head);
int mx_ush_launch(t_global *hd, t_lst *head);
char **mx_ush_split_line(char *line, char *delim);
t_lst *mx_ush_read_line(t_cmd_history **hist, t_global **hd);

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
//
void mx_print_env(char **env);
//
char **mx_parse_env_args(t_global **hd);
//функція виконує додатковий парсинг якщо в аргументі міститься ;
t_lst *mx_additional_parsing(t_lst **head, char *arg);
//функція виводить помилку usage для env
void mx_usage_env(char *flag);
//функція ініціалізує структуру локальних змінних нульовими значеннями
char **init_vars();
//функція додає змінну та її значення із строки до локальних змінних
void add_local_var(t_global **hd, char *str);
//фрішить глобальну структуру
void delete_global(t_global *head);
//реалізація вбудованої команди echo
int mx_echo(t_global *hd, t_lst *head);
////функція перевіряє чи менша строка входить в більшу (до знака = в більшій)
bool find_var_in_str(char *big, char *little);
//функція для експортування змінних в ENV
int mx_export(t_global *hd, t_lst *head);
//вбудована функція cd
int mx_cd(t_global *hd, t_lst *head);
//функція приймає ім'я змінної і шукає її перше входження в nev(до =) і повертає індекс
int search_for_var_in_env(t_global *hd, char *str);
//те саме що і верхня тілкьки з масивом vars
int search_for_var_in_vars(t_global *hd, char *str);
//видаляє змінні із env та var
int mx_unset(t_global *hd, t_lst *head);


// ailchuk
int mx_exit(t_global *hd);
int mx_builtin_pwd(t_global *hd, t_lst *head);
char **mx_env_copy(void); // копирует все из env
char *mx_get_env_var(char **env, char *var);   // возвращает ключ переменной который ты ищешь в массиве ЕНВ
void mx_set_env_var(char *key, char *value, char ***env); // PWD, /usr/ailchuk env меняет енв
int mx_chdir_l(char *path, char flags);


#endif
