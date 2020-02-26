#include "ush.h"

int main() {
    t_global *head = malloc(sizeof(t_global *));

    head->env = mx_env_copy();
    mx_ush_loop(head);         //цикл команд
    // free(a);
}
