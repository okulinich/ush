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
#include <dirent.h>
#include <spawn.h>

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

typedef struct s_dirs {
    char *pwd;
    char *oldpwd;
    char *home;
}   t_dirs;


typedef struct s_lst {
    char *cmd; //"ls"
    char **av; //{ "ls" , "-la", "src", null}
    struct s_lst *next;
}   t_lst;

typedef struct s_cmd_history {
    char *user_input;
    struct s_cmd_history *next;
    struct s_cmd_history *prev;
}   t_cmd_history;

typedef struct s_global {
    t_lst *new;
    char **env;
    char **vars;
    char *input;
    int last_exit_status;
}   t_global;

typedef struct s_spawn_args {
    pid_t child;
    int fds[2];
    posix_spawn_file_actions_t actions;
    int status;
} t_spawn_args;


// core
void mx_ush_loop(t_global *hd);
int mx_ush_execute(t_global *hd, t_lst *head);
int mx_ush_launch(t_global *hd, t_lst *head);
char **mx_ush_split_line(char *line, char *delim);
t_lst *mx_ush_read_line(t_global *hd, char *input);

//зчитування, парсинг строки, формування списку команд для виконання
t_lst *lsh_read_line(t_cmd_history **hist);
//переводить термінал в неканонічний режим, зчитує строку, опрацьовує сигнали
char *mx_noncanon_read_line(void);
int mx_read_from_stdin(char **line, int *i, bool *errow_pressed, 
                       struct termios *savetty);
void mx_switch_noncanon(struct termios *savetty, struct termios *tty);
void mx_switch_canon(struct termios *savetty);
//функція що перевіряє чи є строка командою
bool mx_is_command(char *str);
//функція що ділить строку на токени
char **ush_split_line(char *line);
//додавання елемента в кінець списку команд
t_lst *push_back(t_lst **head, char *command);
//void push_back(t_lst **head, char *command);
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
bool backsp(char *ch, char **line, int *i);
//опрацювання клавіш-стрілок праворуч і ліворуч
bool left_right_key(char ch[4], char **line, int *i);
//перевіряє чи була натиснута клавіша за її кодом
bool arrow_pressed(char *str, int a, int b, int c);
//заповнює строку '\0'
void mx_line_alloc(char *line);
//prints environ array
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
int mx_export(t_lst *head);
//вбудована функція cd
int mx_cd(t_global *hd, t_lst *head);
//функція приймає ім'я змінної і шукає її перше входження в nev(до =) і повертає індекс
int search_for_var_in_env(char *str);
//те саме що і верхня тілкьки з масивом vars
int search_for_var_in_vars(t_global *hd, char *str);
//видаляє змінні із env та var
int mx_unset(t_lst *head) ;
//розділяє строку по лапках
char **mx_split_by_quotes(char *line);
//додає або замінює змінну в env масиві
void add_var_to_env(char *name, char *value, t_global *hd);
//built-in which command
int mx_which(t_lst *head);
//якщо знаходить змінну повертає *=value* інакше = 0
char *ssearch_for_var_in_env(char *str);
//ekranirovanie of space-symbols
void catch_escape_seq(char *str); 
//checks array of args for `cmd`
int check_cmd_args_for_commands(t_lst *cur);
//return a string that contains output from `cmd`
char *get_cmd_output(char *cmd, t_global *hd);
//special parser for command substitution `cmd`
t_lst *parse_string(char *line);
//splits each string by the delimiter
void split_by_delimiter(char ***av);
//fills the list with values (commands and arguments)
void fill_cmd_list(char **global, t_lst **head);
//creates an item of the list
t_lst *create_node(char *str);
//returns char c + '\'
char get_escape(char c);
//adds new var and it's value to environ array
void add_str_to_env(char *str);
//checks the str for variables and replaces vars with their values
bool get_var_from_str(char **str);
//returns the value of variable from environ array
char *search_for_var(char *str);
//
void mx_repl_quotes_with_cmd(char **cmd, t_global *hd);
//
bool mx_replace_pharent_with_quotes(char *line);
//
bool check_if_cmd_is_builtin(char *cmd);
//
bool mx_string_has_chars(char *str);

// ailchuk
int mx_exit(t_global *hd, t_lst *l);
int mx_builtin_pwd(t_lst *head);
char **mx_env_copy(void); // копирует все из env
char *mx_get_env_var(char **env, char *var);   // возвращает ключ переменной который ты ищешь в массиве ЕНВ
void mx_set_env_var(char *key, char *value, char ***env); // PWD, /usr/ailchuk env меняет енв
int mx_cd_p(char *path, char flags, t_dirs *d);
int mx_cd_l(char *path, char flags, t_dirs *d);
char *mx_add_one_slash(char *path, char *new_part);
char *mx_del_last_slash(char *path);

void mx_nosig(void);
void mx_handler(); // signals
int mx_help_command(t_global *g, t_lst *l); // built 
int mx_call_true(t_global *h); // b
int mx_call_false(t_global *h); // b 
int mx_set(t_lst *s); // b 
int mx_bye(t_global *g, t_lst *h); // b 
int mx_return(t_global *g, t_lst *l); // b

#endif
