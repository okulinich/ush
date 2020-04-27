#include "ush.h"

int get_line(char **line) {
    int res = 0;

    for(int i = 0; i < 255; i++)
        read(0, &(*line)[i], 1);
    return 1;
}

int main(int argc, char *argv[]) {
    t_global *head = malloc(sizeof(t_global *));

    char *line = mx_strnew(255);
    size_t buf = 0;
    if(isatty(0) == 0 && get_line(&line) > 0)
        head->input = line;
    else
         head->input = NULL;

    //head->env = mx_env_copy();                  //системні змінні оболонки
    //head->last_exit_status = 0;
    // head->vars = init_vars();                   //локальні змінні оболонки /// ЛИКУЕТ!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //add_var_to_env("SHLVL", "=2", head);
    mx_ush_loop(head);                          //основний цикл
    //delete_global(head);                        //НЕ ЧИСТИТЬСЯ, ТРЕБА ЦЮ ФУНКЦІЮ
                                                //ВИКЛИКАТИ ПЕРЕД EXIT
}
