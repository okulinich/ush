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

char *lsh_read_line() {
    char *line = NULL;
    char **av = NULL;
    size_t bufsize = 0;
    t_lst *head;

/*                                   * тут потрібно викликати функцію, яка     */
    getline(&line, &bufsize, stdin); /* буде переводити термінал в неканонічний */
                                     /* режим, опрацьовувати сигнали та змінні  */
/*                                   * і повертати строку                      */
/*    */
    av = ush_split_line(line);      /* розділяємо строку на токени  */ 
    // mx_is_command(av[0]));       /* формуємо список із команд і аргументів ;/
    return line;
}

bool mx_is_command(char *str) {
    int j = 0;
    bool flag;

    for (int i = 0; COMMANDS[i] != '\0'; i++) {
        if(COMMANDS[i] == str[j]) {
            flag = true;
            while(str[j] != '\0') {
                if(str[j++] != COMMANDS[i++]) {
                    flag = false;
                    break;
                }
            }
            if(flag)
                return true;
            else
                i -= j - 1;
        }
    }
    return false;
}
