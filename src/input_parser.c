#include "ush.h"

// char *lsh_read_line() {
//     struct termios savetty;             //змінні для зберігання управляючих струтктур
//     struct termios tty;
//     char *line = (char *)malloc(sizeof(char) * BUFSIZE);
//     char ch;
//     int i = 0;

//     if(!isatty(0)) {
//         fprintf(stderr, "stdin is not terminal!\n");
//         exit(1);
//     }

//     switch_noncanon(&savetty, &tty);

//     for(;;) {
//         read (0, &ch, 1);
//         if (ch == 'q')
//             break;
//         switch (ch) {
//             case 'w':
//                 printf ("%c[1A", 27);
//                 fflush (stdout);        //update stream
//                 break;
//             case 's':
//                 printf ("%c[1B", 27);
//                 fflush (stdout);
//                 break;
//             case 'f':
//                 printf ("%c[1C", 27);
//                 fflush (stdout);
//                 break;
//             case 'a':
//                 printf ("%c[1D", 27);
//                 fflush (stdout);
//                 break;
//             default:
//                 printf("%c", ch);
//                 fflush(stdout);
//                 break;
//         }
//     }

//     switch_canon(&savetty);
//     exit(0);
// }

t_lst *lsh_read_line(t_cmd_history **hist) {
    char *line = NULL;
    char **av = NULL;
    t_lst *head = NULL;
    t_lst *tmp = NULL;
    int i = 0;

/*                                   * тут потрібно викликати функцію, яка     */
//  getline(&line, &bufsize, stdin); /* буде переводити термінал в неканонічний */
                                     /* режим, опрацьовувати сигнали та змінні  */
/*                                   * і повертати строку                      */
/*    */

    line = noncanon_read_line(hist);

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
