#include "ush.h"

int main() {
    t_global *head = malloc(sizeof(t_global *));

    head->env = mx_env_copy();                  //системні змінні оболонки
    // head->vars = init_vars();                   //локальні змінні оболонки /// ЛИКУЕТ!!!!!!!!!!!!!!!!!!!!!!!!!!!
    add_var_to_env("SHLVL", "=2", head);
    mx_ush_loop(head);                          //основний цикл
    delete_global(head);                        //НЕ ЧИСТИТЬСЯ, ТРЕБА ЦЮ ФУНКЦІЮ
                                                //ВИКЛИКАТИ ПЕРЕД EXIT
}
