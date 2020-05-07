#include "ush.h"

int main() {
    t_global *head = malloc(sizeof(t_global));
    char *line;
    size_t buf = 0;
    head->input = NULL;
    
    mx_nosig();    
    if(isatty(0) == 0) {
        //mx_printstr("not chr device\n");
        if(getline(&line, &buf, stdin) < 0) {
            //mx_printstr("no input str\n");
            exit(1);
        }
        head->input = mx_strnew(1024);
        mx_strcpy(head->input, line);
    }
    else {
        head->input = mx_strnew(1024);
        mx_strcpy(head->input, "emptyinput");
    }

    //head->env = mx_env_copy();                  //системні змінні оболонки
    //head->last_exit_status = 0;
    // head->vars = init_vars();                   //локальні змінні оболонки /// ne yuzaem!!!
    //add_var_to_env("SHLVL", "=2", head);
    setenv("SHLVL", "2", 1);
    // mx_ush_loop(head, input);                          //основний цикл
    mx_ush_loop(head);                          //основний цикл
    //delete_global(head);                        //НЕ ЧИСТИТЬСЯ, ТРЕБА ЦЮ ФУНКЦІЮ // VRODE NE YUZAEM ETO 
                                                //ВИКЛИКАТИ ПЕРЕД EXIT
}
