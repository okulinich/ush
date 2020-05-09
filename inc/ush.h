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

#define MX_BUFSIZE 1024
#define MX_DELIMITERS "\t\r\n\a "
#define MX_LOOP_BREAK 1
#define MX_RETURN_EMPTY 2
#define MX_LOOP_CONTINUE 3

typedef struct s_dirs {
    char *pwd;
    char *oldpwd;
    char *home;
}              t_dirs;

typedef struct s_lst {
    char *cmd; //"ls"
    char **av; //{ "ls" , "-la", "src", null}
    struct s_lst *next;
}              t_lst;

typedef struct s_global {
    t_lst *new;
    char **env;
    char **vars;
    char *input;
    int last_exit_status;
}              t_global;

typedef struct s_spawn_args {
    pid_t child;
    int fds[2];
    posix_spawn_file_actions_t actions;
    int status;
}              t_spawn_args;


typedef struct s_counters {
    int pos;
    int amount;
    int j;
    int i;
}              t_counter;

typedef struct s_indexes {
    int indx;
    int end_indx;
    int j;
}              t_index;

// core
void mx_ush_loop(t_global *hd);
int mx_ush_execute(t_global *hd, t_lst *head);
int mx_ush_launch(t_global *hd, t_lst *head);
char **mx_ush_split_line(char *line, char *delim);
t_lst *mx_ush_read_line(char *input);

//переводить термінал в неканонічний режим, зчитує строку, опрацьовує сигнали
char *mx_noncanon_read_line(void);
int mx_read_from_stdin(char **line, int *i, bool *errow_pressed, 
                       struct termios *savetty);
void mx_switch_noncanon(struct termios *savetty, struct termios *tty);
void mx_switch_canon(struct termios *savetty);
//функція що ділить строку на токени
char **ush_split_line(char *line);
//додавання елемента в кінець списку команд
t_lst *mx_push_back_lst(t_lst **head, char *command);
//видалення списку команд і аргументів
void mx_delete_list(t_lst *head);
//додає аргумент до поточної команди в списку
void mx_add_new_arg(t_lst *tmp, char *arg);
//опрацювання історії та backspace
bool mx_if_backsp(char *ch, char **line, int *i);
//опрацювання клавіш-стрілок праворуч і ліворуч
bool mx_left_right_key(char ch[4], char **line, int *i);
//перевіряє чи була натиснута клавіша за її кодом
bool mx_arrow_pressed(char *str, int a, int b, int c);
//заповнює строку '\0'
void mx_line_alloc(char *line);
//prints environ array
void mx_print_env(char **env);
char **mx_parse_env_args(t_global **hd);
//функція виводить помилку usage для env
void mx_usage_env(char *flag);
//реалізація вбудованої команди echo
int mx_echo(t_global *hd, t_lst *head);
////функція перевіряє чи менша строка входить в більшу (до знака = в більшій)
bool mx_find_var_in_str(char *big, char *little);
//функція для експортування змінних в ENV
int mx_export(t_lst *head);
//вбудована функція cd
int mx_cd(t_global *hd, t_lst *head);
//функція приймає ім'я змінної і шукає її перше входження в nev(до =) і повертає індекс
int mx_search_for_var_in_env(char *str);
//видаляє змінні із env та var
int mx_unset(t_lst *head) ;
//розділяє строку по лапках
char **mx_split_by_quotes(char *line);
//built-in which command
int mx_which(t_lst *head);
char *mx_get_path_to_binary(char *bin_name, bool search_all_bins);
//якщо знаходить змінну повертає *=value* інакше = 0
char *mx_ssearch_for_var_in_env(char *str);
//ekranirovanie of space-symbols
void mx_catch_escape_seq(char *str); 
//checks array of args for `cmd`
int mx_check_cmd_args_for_commands(t_lst *cur);
//return a string that contains output from `cmd`
char *mx_get_cmd_output(char *cmd, t_global *hd);
//special parser for command substitution `cmd`
t_lst *mx_parse_string(char *line);
//fills the list with values (commands and arguments)
void mx_fill_cmd_list(char **global, t_lst **head);
//creates an item of the list
t_lst *mx_create_lst_node(char *str);
//adds new var and it's value to environ array
void mx_add_str_to_env(char *str);
//checks the str for variables and replaces vars with their values
bool mx_get_var_from_str(char **str);
//returns the value of variable from environ array
char *mx_search_for_var(char *str);
//replace string in such `` quotes with the result of executing cmd from ``
void mx_repl_quotes_with_cmd(char **cmd, t_global *hd);
//replace such $() with simple `` quotes (they are the same)
bool mx_replace_pharent_with_quotes(char *line);
//check if command is built-in in out shell (for which -s)
bool mx_check_if_cmd_is_builtin(char *cmd);
//check if string consist only of space-symblols or not
bool mx_string_has_chars(char *str);
//split line by ;
void mx_split_by_delimiter(char ***av);
//replaces string from av[] with arr of strings if it includes ';'
void mx_replace_arg_with_arr(char ***av, int indx, char **str_arr);
//returns index of substring in array of strings or -1
int mx_get_substring_index(char **arr, char *str);
//створює новий масив змінних із тих що ввів користувач (після флага -і) для передачі їх в EXECVE
char **mx_fill_new_env(t_global **hd, int *i);
//check whether there are arguments after P flag in env and perform some actions
bool mx_check_p_flag_args(t_global **hd, char **folder_to_search, int *i);
///функція правильно розприділяє аргументи команди ENV для подальшої передачі в EXECVE///
void mx_reparse_input_for_env(t_global **hd, int i, char *folder_to_search);
//функція для реалізації -u (ігнорує записані користувачем змінні)
char **mx_ignore_variables(t_global **hd);
//check wether semicolons are placed correctly (bash-like style)
bool mx_semicol_placed_right(char **global, t_lst **head, int j);
//checks if all types of qoutes are used correctly
bool mx_parse_quotes(char *line, char ***av);
//additional parsing of quotes
void mx_parse_str_with_quotes(char ***global, char ***av);
//free used in parser memory
void mx_free_used_memory(char *line, char **av);
//check string for escape seq and split str by semicolons
void mx_escape_seq_and_split_by_semicol(char *line, char ***av);
//print argument passed to echo with escape characters
void mx_print_with_escape(char *str);
//return value of the variable detected in echo arg
char *mx_get_variable_value(int pos, char *str);
//replaces variable in echo arg with it's value
void mx_replace_var_with_valuev(char **str, char *var_value, int i);
//finds amount of quotees in line and type of first qoute
int mx_count_quotes(char *line, char *quote_type);
//increment SHLVL value
void mx_set_correct_shlvl(void);
//create new proc with posix_spawn
void mx_create_new_proccess(t_spawn_args *spwn, t_lst *head);
//read from stdout created by pipe() and passed to posix_spawn
void mx_get_bytes_from_pipe(t_spawn_args *spwn, char **str, t_global *hd);
//if str has more commands in `` qoutes - get their output
void mx_continue_execution(t_spawn_args *spwn, t_lst **head, char **str, t_global *hd);
//stop getting output of command in `` and free used memory
char *mx_end_execution(char **str, t_lst *root, int cur_case);
//parse command from `` and fill list (for execution of a command)
t_lst *mx_parse_cmd_to_list(char *cmd);
//copy arg if command like `echo smth`
char *mx_copy_echo_arg(t_lst *head, t_lst *root, int arg);
//add tokens (created by spliting str) to array
void mx_add_tokens_to_arr(char ***global, char ***temp, int *gl_i);
int mx_exit(t_global *hd, t_lst *l);
int mx_builtin_pwd(t_lst *head);
char **mx_env_copy(void); // копирует все из env
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
