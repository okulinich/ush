#include "ush.h"

t_lst *lsh_read_line(t_cmd_history **hist) {
    char *line = NULL;
    char **av = NULL;
    t_lst *head = NULL;
    t_lst *tmp = NULL;
    int i = 0;

    line = noncanon_read_line(hist);

    av = ush_split_line(line);      /* розділяємо строку на токени  */

    while (av[i] != NULL) {
        if (mx_is_command(av[i])) {         /* якшо строка це команда, тоді додаємо */
                                            /* команду в список */
            tmp = push_back(&head, av[i]);
            while(av[++i] && strcmp(av[i], "&&") != 0) {   /* доки строка не команда -> записуємо */
                add_new_arg(tmp, av[i]);                            /* строку як аргумент до команди */
            }
        }
        else {
            push_back(&head, av[i]);
            ++i;
        }
    }

    free(line);
    free(av);
    return head;
}

bool mx_is_command(char *str) {
    char **comands = get_commands();            //потрібно потім ЗАФРІШИТИ!!
    
    for(int i = 0; i < COMMANDS; i++) {
        if(strcmp(str, comands[i]) == 0) {
            free(comands);
            return true;
        }
    }
    free(comands);
    return false;
}

char **get_commands() {
    char **comands = (char **)malloc(sizeof(char *) * COMMANDS);

    comands[0] = "export";
    comands[1] = "unset";
    comands[2] = "fg";
    comands[3] = "exit";
    comands[4] = "env";
    comands[5] = "cd";
    comands[6] = "pwd";
    comands[7] = "which";
    comands[8] = "echo";
    comands[9] = "ls";
    return comands;
}

char **fill_new_env(t_global **hd, int *i) {
    char **new_env = (char **)malloc(sizeof(char *));
    int j = 0;

    new_env[0] = NULL;

    for(*i = 2; mx_get_char_index((*hd)->new->av[*i], '=') > 0; (*i)++) { //формуємо список із нових аргументів доки зустрічаємо '=' в строці
        while(new_env[j] != NULL)
            j++;
        new_env = realloc(new_env, (sizeof(char *)) * (j + 2));
        new_env[j] = mx_strdup((*hd)->new->av[*i]);
        new_env[j + 1] = NULL;
    }
    return new_env;
}

void reparse_input_for_env(t_global **hd, int i) {
    char *filename;
    char **buf = NULL;
    int j = 0;

    filename = mx_strdup((*hd)->new->av[i]);
    buf = (char **)malloc(sizeof(char *) * BUFSIZE);
    buf[0] = NULL;
    while((*hd)->new->av[++i])
    buf[j++] = mx_strdup((*hd)->new->av[i]);          //зберігаємо флаги якщо вони є
    if((*hd)->new->cmd) {
        mx_del_strarr(&(*hd)->new->av);
        free((*hd)->new->cmd);
    }
    (*hd)->new->cmd = mx_strjoin("/bin/", filename);        //записуємо шляха до бінарника
    (*hd)->new->av = (char **)malloc(sizeof(char *) * 2);   //формуємо масив аргументів
    (*hd)->new->av[0] = mx_strdup(filename);
    (*hd)->new->av[1] = NULL;

    for(j = 0; buf[j]; j++)
        add_new_arg((*hd)->new, buf[j]);
    mx_del_strarr(&buf);
}

char **mx_parse_env_args(t_global **hd) {
    int i;
    char **new_env;

    if(!(*hd)->new->av[1]) {          //якщо env без флагів тоді просто виводимо змінні оболонки
        mx_print_env((*hd)->env);
        return NULL;
    }
    else if(mx_strcmp((*hd)->new->av[1], "-i") == 0) {
        if(!(*hd)->new->av[2])          //якщо немає аргументів після 'і' тоді нічого не робимо 
            return NULL;
        else {
            new_env = fill_new_env(hd, &i);     //заповнюємо новий масив із введених змінних
            if((*hd)->new->av[i]) {             //якщо далі є команда яку потрібно виконати тоді 
                reparse_input_for_env(hd, i);   //передаємо їй новий масив і виконуємо цю команду в execve()
                return new_env;
            }
            else {                              //якщо команди немає, тоді просто виводимо
                mx_print_env(new_env);          //введені користувачем змінні
                mx_del_strarr(&new_env);
                return NULL;
            }
        }
    }
    else if(mx_strcmp((*hd)->new->av[1], "-u") == 0) {
        mx_printstr("Опрацьовуємо флаг -у\n");
        return NULL;
    }
    else if(mx_strcmp((*hd)->new->av[1], "-P") == 0){
        mx_printstr("Опрацьовуємо флаг -Р\n");
        return NULL;
    }
    else {
        reparse_input_for_env(hd, 1);
        new_env = mx_env_copy();
        return new_env;
    }
    return NULL;
}
