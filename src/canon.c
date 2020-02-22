#include "ush.h"

void switch_noncanon(struct termios *savetty, struct termios *tty) {
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

void switch_canon(struct termios *savetty) {
/*                                          */
    tcsetattr (0, TCSAFLUSH, savetty);
/*          канонічний режим відновлено      */
//TCSAFLUSH - це набір макросів, його потрібно задефайнити
}


void print_next_cmd(t_cmd_history **cur, char **line) {
    if(*cur) {
        write(1, (*cur)->user_input, strlen((*cur)->user_input));
        *line = mx_strdup((*cur)->user_input);
        if((*cur)->next)
            *cur = (*cur)->next;
    }
}

void print_prev_cmd(t_cmd_history **cur, char **line) {
    if(*cur && (*cur)->prev->prev) {
        *cur = (*cur)->prev;
        write(1, (*cur)->prev->user_input, strlen((*cur)->prev->user_input));
        *line = mx_strdup((*cur)->prev->user_input);
    }
}

char *noncanon_read_line(t_cmd_history **head) {
    struct termios savetty;             //змінні для зберігання управляючих струтктур
    struct termios tty;
    char *line = (char *)malloc(sizeof(char) * BUFSIZE);
    char ch[4] = {'\0', '\0', '\0', '\0'};
    int i = 0;
    t_cmd_history *cur = *head;

    switch_noncanon(&savetty, &tty);

    write(1, "u$h> ", 5);
    for(;;) {
        read(0, ch, 4);
        if(ch[0] == '\n')
            break;
        else if(ch[0] == 3) {
            mx_printchar('\n');
            return "";          //freee!!
        }
        else if(hist_or_backsp(ch, &cur, &line, &i)) {
            continue;
        }
        // else if(strlen(ch) > 1) {
        //     mx_printstr("here!!");
        //     continue;
        // }
        else {
            if(i >= BUFSIZE)
                line = realloc(line, sizeof(char) * (i + BUFSIZE));
            line[i++] = ch[0];
            write(1, &ch[0], 1);
        }
    }

    if(strlen(line) >= 1) {
        push_front_history(head, line);
    }

    switch_canon(&savetty);
    mx_printchar('\n');
    return line;
}

bool hist_or_backsp(char *ch, t_cmd_history **cur, char **line, int *i) {
    bool res = false;
    char esc = 27;

    if(arrow_pressed(ch, 27, 91, 65)) {
        if(*cur) {
            while(strlen(*line) > 0)
                backspace(esc, line);
            print_next_cmd(cur, line);
            *i = strlen(*line) - 1;
        }
        res = true;
    }
    else if(arrow_pressed(ch, 27, 91, 66)) {
        if(*cur) {
            while(strlen(*line) > 0)
                backspace(esc, line);
            print_prev_cmd(cur, line);
            *i = strlen(*line);
        }
        res = true;
    }
    else if (ch[0] == 127) {
        backspace(esc, line);
        (*i)--;
        res = true;
    }
    return res;
}

void backspace(char ch, char **line) {
    char *move_left = mx_strjoin(" ", "[1D");
    char *tmp = mx_strnew(strlen(*line) - 1);

    if(strlen(*line) == 0) {
        free(move_left);
        free(tmp);
        return;
    }
    move_left[0] = ch;
    mx_printstr(move_left);
    mx_printchar(' ');
    mx_strncpy(tmp, *line, strlen(*line) - 1);
    free(*line);
    *line = tmp;
    mx_printstr(move_left);

    free(move_left);
}

bool arrow_pressed(char *str, int a, int b, int c) {
    if(strlen(str) == 3 && str[0] == a && str[1] == b && str[2] == c)
        return true;
    else
        return false;
}
