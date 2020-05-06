#include "ush.h"

static void check_write(bool *errow_pressed, char **line, int *i, char *move_left, int j) {
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
    check_write(errow_pressed, line, i, move_left, j);
}

static int ctrl_c_or_d(char *ch, struct termios *savetty) {
    if (ch[0] == 4) { // ctrl d
        mx_printstr("\n");
        mx_switch_canon(savetty);
        exit(0);
        return LOOP_BREAK;
    }
    else if (ch[0] == 3) { // ctrl c
        mx_printchar('\n');
        return RETURN_EMPTY;          //freee!!
    }
    return LOOP_CONTINUE;
}

int mx_read_from_stdin(char **line, int *i, bool *errow_pressed, 
                       struct termios *savetty) {
    char ch[4] = {'\0', '\0', '\0', '\0'};             //буфер для посимвольного зчитування

    read(0, ch, 4);
    if(ch[0] == '\n')
        return LOOP_BREAK;          //після натискання Enter припиняємо зчитування
    else if(ch[0] == 4 || ch[0] == 3)           //відловлюємо ctrl + c
        return ctrl_c_or_d(ch, savetty);
    else if (ch[0] == 26) 
        write(1, "\a", 1);
    else if(backsp(ch, line, i))      //опрацьовуємо бекспейс
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

