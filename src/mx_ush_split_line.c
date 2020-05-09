#include "ush.h"

void mx_catch_escape_seq(char *str) {
    char *temp = NULL;
    int j = 0;

    for (int i = 0; i < mx_strlen(str); i++) {
        if (str[i] == '\\' && i + 1 < mx_strlen(str) && str[i + 1] != ' ') {
            temp = mx_strdup(str);
            j = 0;
            for (int g = 0; g < mx_strlen(temp); g++)
                if (g != i)
                    str[j++] = temp[g];
            str[j] = '\0';
            free(temp);
        }
    }
}

void mx_replace_space(char *line) {
    for (int i = 0; i < mx_strlen(line); i++)   
        if (line[i] == '\\' && i + 1 < mx_strlen(line) && line[i + 1] == ' ')
            line[i + 1] = '_';
}

void mx_add_token_to_strarr(char *token, char **tokens, int *pos, int *j) {
    for (int i = 0; i < mx_strlen(token); i++) {
        if (token[i] == '\\' && i + 1 < mx_strlen(token) 
            && token[i + 1] == '_') {
            tokens[*pos][(*j)++] = ' ';
            i++;
        }
        else
            tokens[*pos][(*j)++] = token[i];
    }
    tokens[(*pos)++][*j] = '\0';
    *j = 0;
}

char **mx_ush_split_line(char *line, char *delim) {
    int buf_size = MX_BUFSIZE;
    int pos = 0;
    char **tokens = (char **)malloc(buf_size * sizeof(char *));      
    char *token;
    int j = 0;

    mx_replace_space(line);
    token = strtok(line, delim == NULL ? MX_DELIMITERS : delim);
    while (token) {
        tokens[pos] = mx_strdup(token);               
        mx_add_token_to_strarr(token, tokens, &pos, &j);
        if (pos >= buf_size) {                        
            buf_size += MX_BUFSIZE;
            tokens = realloc(tokens, buf_size * (sizeof(char *)));
        }
        token = strtok(NULL, delim == NULL ? MX_DELIMITERS : delim);
    }
    tokens[pos] = NULL;
    return tokens;
}

