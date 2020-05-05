#include "ush.h"

void catch_escape_seq(char *str) {
    char *temp = NULL;
    int j = 0;

    for(int i = 0; i < mx_strlen(str); i++) {
        if(str[i] == '\\' && i + 1 < mx_strlen(str) && str[i + 1] != ' ') {
            temp = mx_strdup(str);
            j = 0;
            for(int g = 0; g < mx_strlen(temp); g++)
                if(g != i)
                    str[j++] = temp[g];
            str[j] = '\0';
            free(temp);
        }
    }
}

char **mx_ush_split_line(char *line, char *delim) {
    int buf_size = BUFSIZE;
    int pos = 0;
    char **tokens = (char **)malloc(buf_size * sizeof(char *));       //масив слів із строки
    char *token;
    int j = 0;

    for(int i = 0; i < mx_strlen(line); i++)    //тимчасово замінюємо екранований спейс на _
        if(line[i] == '\\' && i + 1 < mx_strlen(line) && line[i + 1] == ' ')
            line[i + 1] = '_';
    token = strtok(line, delim == NULL ? DELIMITERS : delim);
    while(token) {
        tokens[pos] = mx_strdup(token);                                  //записуємо кожне слово в масив
        //tokens[pos] = token;                                  //записуємо кожне слово в масив
        for(int i = 0; i < mx_strlen(token); i++)
            if (token[i] == '\\' && i + 1 < mx_strlen(token) && token[i + 1] == '_') {
                tokens[pos][j++] = ' ';
                i++;
            }
            else
                tokens[pos][j++] = token[i];
        tokens[pos][j] = '\0';
        j = 0;
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
    int third = mx_get_char_index(line, '`');
    
    if(ordinar > 0 && line[ordinar - 1] == '\\')
        ordinar = -1;
    if(doubl > 0 && line[doubl - 1] == '\\')
        doubl = -1;
    if(third > 0 && line[third - 1] == '\\')
        third = -1;

    *quote_type = 'q';
    if(ordinar >= 0 && doubl >= 0 && third >= 0)
        *quote_type = ordinar < doubl ? (ordinar < third ? '\'' : '`') : (doubl < third ? '\"' : '`');
    else if(ordinar >= 0 && doubl >= 0)
        *quote_type = ordinar < doubl ? '\'' : '\"';
    else if(doubl >= 0 && third >= 0)
        *quote_type = doubl < third ? '\"' : '`';
    else if(ordinar >= 0 && third >= 0)
        *quote_type = ordinar < third ? '\'' : '`';
    else if(ordinar >= 0 || doubl >= 0 || third >= 0)
        *quote_type = ordinar >= 0 ? '\'' : ( doubl >= 0 ? '\"' : '`');
    if(*quote_type == 'q')
        return 0;
    else {
        ordinar = 0;
        for(int i = 0; i < mx_strlen(line); i++)
            if(line[i] == *quote_type && (i == 0 || line[i - 1] != '\\'))
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
                while (line[pos] != quote_type || (pos - 1 >= 0 && line[pos - 1] == '\\')) {
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
