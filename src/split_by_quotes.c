#include "ush.h"


void mx_init_counters(t_counter *c, char *line, char *qoute_type) {
    c->pos = 0;
    c->amount = mx_count_quotes(line, qoute_type);
    c->j = 0;
    c->i = 0;
}

void mx_add_token(t_counter *count, char *line, 
                    char quote_type, char **tokens) {
    for (int g = 0; g < count->amount; g++) {
        tokens[count->i] = mx_strnew(MX_BUFSIZE);
        if (count->i % 2 != 0)
            tokens[count->i][count->j++] = line[count->pos++];
        else if (count->i != 0)
            count->pos++;
        while (line[count->pos] != quote_type || (count->pos - 1 >= 0 
                && line[count->pos - 1] == '\\')) {
            tokens[count->i][count->j++] = line[count->pos++];
        }
        if (count->i % 2 != 0)
            tokens[count->i][count->j++] = line[count->pos];
        count->j = 0;
        count->i++;

    }
    if (count->pos <= mx_strlen(line))
        count->amount = mx_count_quotes(&line[count->pos + 1], &quote_type);
    else
        count->amount = 0;
}

char **mx_split_by_quotes(char *line) {
    t_counter count;
    char quote_type;
    char **tokens = (char **)malloc(MX_BUFSIZE * sizeof(char *));    
    
    mx_init_counters(&count, line, &quote_type);
    if (count.amount == 0) {
        free(tokens);
        return NULL;
    }
    else if (count.amount == -1) {
        tokens[0] = "ERROR";
        return tokens;
    }
    else {
        while (count.amount > 0)
            mx_add_token(&count, line, quote_type, tokens);
        if (count.pos + 1 < mx_strlen(line))
            tokens[count.i] = mx_strdup(&line[count.pos + 1]);
        return tokens;
    }
}
