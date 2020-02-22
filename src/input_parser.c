#include "ush.h"

t_lst *lsh_read_line(t_cmd_history **hist) {
    char *line = NULL;
    char **av = NULL;
    size_t bufsize = 0;
    t_lst *head = NULL;
    t_lst *tmp = NULL;
    int i = 0;

/*                                   * тут потрібно викликати функцію, яка     */
    //getline(&line, &bufsize, stdin); /* буде переводити термінал в неканонічний */
                                     /* режим, опрацьовувати сигнали та змінні  */
/*                                   * і повертати строку                      */
/*    */
    line = noncanon_read_line(hist);    /* зчитуємо строку в режимі неканонічного 
                                       вводу */
    av = ush_split_line(line);      /* розділяємо строку на токени  */ 

    while (av[i] != NULL) {
        if (mx_is_command(av[i])) {         /* якшо строка це команда, тоді додаємо */
                                            /* команду в список */
            tmp = push_back(&head, av[i]);
            while(av[++i] && !mx_is_command(av[i]) && strcmp(av[i], "&&") != 0) {   /* доки строка не команда -> записуємо */
                add_new_arg(tmp, av[i]);                            /* строку як аргумент до команди */
            }
        }
        else {
            push_back(&head, av[i]);
            ++i;
        }
    }

    return head;
}

bool mx_is_command(char *str) {
    char **comands = get_commands();            //потрібно потім ЗАФРІШИТИ!!

    for(int i = 0; i < COMMANDS; i++) {
        if(strcmp(str, comands[i]) == 0)
            return true;
    }
    return false;
}

char **get_commands() {
    char **comands = (char **)malloc(sizeof(char *) * COMMANDS);

    comands[0] = strdup("export");
    comands[1] = strdup("unset");
    comands[2] = strdup("fg");
    comands[3] = strdup("exit");
    comands[4] = strdup("env");
    comands[5] = strdup("cd");
    comands[6] = strdup("pwd");
    comands[7] = strdup("which");
    comands[8] = strdup("echo");
    comands[9] = strdup("ls");
    return comands;
}