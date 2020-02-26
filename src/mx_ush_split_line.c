#include "ush.h"

char **mx_ush_split_line(char *line) {
    int buf_size = BUFSIZE;
    int pos = 0;
    char **tokens = (char **)malloc(buf_size * sizeof(char *));       //масив слів із строки
    char *token;

    token = strtok(line, DELIMITERS);
    while(token) {
        tokens[pos] = token;                                  //записуємо кожне слово в масив
        pos++;
        if(pos >= buf_size) {                           //розширюємо масив слів якшо потрібно
            buf_size += BUFSIZE;
            tokens = realloc(tokens, buf_size * (sizeof(char *)));
        }
        token = strtok(NULL, DELIMITERS);
    }
    tokens[pos] = NULL;
    return tokens;
}
