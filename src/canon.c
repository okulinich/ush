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
