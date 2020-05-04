#include "ush.h"

int main() {
    t_cmd_history *hist = NULL;

    char *line = noncanon_read_line(&hist);
    
    char *str = mx_strdup(line);

    printf("Readed string = *%s*\n", str);

    return 0;
    // t_global *head = malloc(sizeof(t_global *));

    // char *line;// = mx_strnew(1024);
    // size_t buf = 0;
    // char *input = NULL;
    // if(isatty(0) == 0) {
    //     mx_printstr("not chr device\n");
    //     if(getline(&line, &buf, stdin) < 0) {
    //         mx_printstr("no input str\n");
    //         exit(1);
    //     }
    //     input = mx_strnew(1024);
    //     mx_strcpy(input, line);
    // }
    // else {
    //     input = mx_strnew(1024);
    //     mx_strcpy(input, "suka");
    // }

    //head->env = mx_env_copy();                  //системні змінні оболонки
    //head->last_exit_status = 0;
    // head->vars = init_vars();                   //локальні змінні оболонки /// ЛИКУЕТ!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //add_var_to_env("SHLVL", "=2", head);
    //mx_ush_loop(head, input);                          //основний цикл
    //delete_global(head);                        //НЕ ЧИСТИТЬСЯ, ТРЕБА ЦЮ ФУНКЦІЮ
                                                //ВИКЛИКАТИ ПЕРЕД EXIT
    
}
