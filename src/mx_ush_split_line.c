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

static int count_quotes(char *line, char *quote_type) {
    int ordinar = mx_get_char_index(line, '\'');
    int doubl = mx_get_char_index(line, '\"');
    
    *quote_type = 'q';
    if(ordinar >= 0 && doubl >= 0)
        *quote_type = ordinar < doubl ? '\'' : '\"';
    else if(ordinar >= 0 || doubl >= 0)
        *quote_type = ordinar >= 0 ? '\'' : '\"';
    if(*quote_type == 'q')
        return 0;
    else {
        ordinar = 0;
        for(int i = 0; i < mx_strlen(line); i++)
            if(line[i] == *quote_type)
                ordinar++;
        if(ordinar % 2 != 0)
            return -1;
        else
            return ordinar;
    }
}

char **mx_split_by_quotes(char *line) {
    int pos = 0;
    int j = 0;
    int i = 0;
    char quote_type;
    int num_of_quotes = count_quotes(line, &quote_type);
    char **tokens = (char **)malloc(BUFSIZE * sizeof(char *));       //масив слів із строки

    if(num_of_quotes == 0) {
        free(tokens);
        return NULL;
    }
    else if(num_of_quotes == -1) {
        tokens[0] = "ERROR";
        return tokens;
    }
    else {
        while(num_of_quotes > 0) {
            for(int g = 0; g < num_of_quotes; g++) {
                tokens[i] = mx_strnew(BUFSIZE);
                if(i % 2 != 0)
                    tokens[i][j++] = line[pos++];
                else if(i != 0)
                    pos++;
                while (line[pos] != quote_type) {
                    tokens[i][j++] = line[pos++];
                }
                if(i % 2 != 0)
                    tokens[i][j++] = line[pos];
                j = 0;
                i++;
            }
            if(pos < mx_strlen(line))
                num_of_quotes = count_quotes(&line[pos + 1], &quote_type);
            else
                num_of_quotes = 0;
        }
        if(pos + 1 < mx_strlen(line))
            tokens[i] = mx_strdup(&line[pos + 1]);
        return tokens;
    }
}
