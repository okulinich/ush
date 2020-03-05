#include "ush.h"

char **mx_ush_split_line(char *line, char *delim) {
    int buf_size = BUFSIZE;
    int pos = 0;
    char **tokens = (char **)malloc(buf_size * sizeof(char *));       //масив слів із строки
    char *token;

    token = strtok(line, delim == NULL ? DELIMITERS : delim);
    while(token) {
        tokens[pos] = mx_strdup(token);                                  //записуємо кожне слово в масив
        pos++;
        if(pos >= buf_size) {                           //розширюємо масив слів якшо потрібно
            buf_size += BUFSIZE;
            tokens = realloc(tokens, buf_size * (sizeof(char *)));
        }
        token = strtok(NULL, delim == NULL ? DELIMITERS : delim);
    }
    tokens[pos] = NULL;
    return tokens;
}
