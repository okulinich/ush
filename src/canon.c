#include "ush.h"

static void switch_noncanon(struct termios *savetty, struct termios *tty) {
    tcgetattr (0, tty);
    *savetty = *tty;                      /* зберегти управляючу інформацію канонічного режиму */
    tty->c_lflag &= ~(ICANON|ECHO|ISIG);
    tty->c_cc[VMIN] = 1;
    tcsetattr (0, TCSAFLUSH, tty);
/*
    з цього моменту канонічний ввід припиняєьться -> вимикається echo-вивід на екран
    і приглушуються всі сигнали
*/
}

static void switch_canon(struct termios *savetty) {
/*                                          */
    tcsetattr (0, TCSAFLUSH, savetty);
/*          канонічний режим відновлено      */
//TCSAFLUSH - це набір макросів, його потрібно задефайнити
}

//%c[1D", 27 - left

static void write_symb_to_str(int *i, char **line, char *ch, bool *errow_pressed) {
    int j = 0;
    char esc = 27;
    char *move_left = mx_strjoin(" ", "[1D");
    
    move_left[0] = esc;
    if(*i >= BUFSIZE)   //sizeof(*line) замість буфера?
        *line = realloc(*line, sizeof(char) * ((*i) + BUFSIZE));
    if(*errow_pressed || (*line)[*i] != '\0') {
        j = strlen(*line);
        (*line)[j + 1] = '\0';
        while(j > *i) {
            (*line)[j] = (*line)[j - 1];
            j--;
        }
    }
    (*line)[(*i)++] = ch[0];                   //записуємо зчитаний символ в строку
    write(1, &ch[0], 1);                    //echo вивід зчитаного символа
    if(*errow_pressed || (*line)[*i] != '\0') {
        j = *i;
        while((*line)[j] != '\0')
            write(1, &(*line)[j++], 1);
        while(--j >= *i)
            mx_printstr(move_left);
        *errow_pressed = false;
    }
    free(move_left);
}

static int read_from_stdin(t_cmd_history **cur, char **line, int *i, bool *errow_pressed) {
    char ch[4] = {'\0', '\0', '\0', '\0'};             //буфер для посимвольного зчитування

    read(0, ch, 4);
    if(ch[0] == '\n')
        return LOOP_BREAK;          //після натискання Enter припиняємо зчитування
    else if(ch[0] == 3) {           //відловлюємо ctrl + c
        mx_printchar('\n');
        return RETURN_EMPTY;          //freee!!
    }
    else if(history_or_backsp(ch, cur, line, i))      //опрацьовуємо історію та бекспейс
        return LOOP_CONTINUE;
    else if(left_right_key(ch, line, i)) {
        *errow_pressed = true;
        return LOOP_CONTINUE;
    }
    else if(ch[1] != '\0')                      //відловлюємо натискання невідомих клавіш
        return LOOP_CONTINUE;
    else
        write_symb_to_str(i, line, ch, errow_pressed);
    return LOOP_CONTINUE;
}

char *noncanon_read_line(t_cmd_history **head) {
    struct termios savetty;             //змінні для зберігання управляючих струтктур
    struct termios tty;                 //
    char *line = mx_strnew(BUFSIZE);
    int i = 0;
    int res = 0;
    t_cmd_history *cur = *head;
    bool errow_pressed = false;         //для реалізації клавіш вліво-вправо

    switch_noncanon(&savetty, &tty);

    write(1, "u$h> ", 5);
    while(1) {
        res = read_from_stdin(&cur, &line, &i, &errow_pressed);
        if(!cur)
            cur = *head;
        if(res == LOOP_BREAK)
            break;
        else if(res == RETURN_EMPTY) {
            free(line);
            mx_strnew(1);
        }
        else if(res == LOOP_CONTINUE)
            continue;
    }

    if(strlen(line) >= 1)                           //добавляємо запис в історію
        push_front_history(head, line);             //якщо це не пуста строка
    
    switch_canon(&savetty);
    mx_printchar('\n');
    return line;
}

