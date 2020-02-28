#include "ush.h"

int main() {
    t_global *head = malloc(sizeof(t_global *));

    head->env = mx_env_copy();                  //системні змінні оболонки
    head->vars = init_vars();                   //локальні змінні оболонки
    mx_ush_loop(head);                          //основний цикл
    delete_global(head);                        //НЕ ЧИСТИТЬСЯ, ТРЕБА ЦЮ ФУНКЦІЮ
                                                //ВИКЛИКАТИ ПЕРЕД EXIT
}
