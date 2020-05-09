#include "ush.h"

void mx_get_first_quote_indx(char *line, int *ordinar, 
                                int *doubl, int *third) {
    if (*ordinar > 0 && line[*ordinar - 1] == '\\')
        *ordinar = -1;
    if (*doubl > 0 && line[*doubl - 1] == '\\')
        *doubl = -1;
    if (*third > 0 && line[*third - 1] == '\\')
        *third = -1;
}

void mx_get_quote_type(int *ordinar, int *doubl, int *third, char *quote_type) {
    if (*ordinar >= 0 && *doubl >= 0 && *third >= 0)
        *quote_type = *ordinar < *doubl ? (*ordinar < *third ? '\'' : '`') 
                                        : (*doubl < *third ? '\"' : '`');
    else if (*ordinar >= 0 && *doubl >= 0)
        *quote_type = *ordinar < *doubl ? '\'' : '\"';
    else if (*doubl >= 0 && *third >= 0)
        *quote_type = *doubl < *third ? '\"' : '`';
    else if (*ordinar >= 0 && *third >= 0)
        *quote_type = *ordinar < *third ? '\'' : '`';
    else if (*ordinar >= 0 || *doubl >= 0 || *third >= 0)
        *quote_type = *ordinar >= 0 ? '\'' : ( *doubl >= 0 ? '\"' : '`');
}

int mx_count_quotes(char *line, char *quote_type) {
    int ordinar = mx_get_char_index(line, '\'');
    int doubl = mx_get_char_index(line, '\"');
    int third = mx_get_char_index(line, '`');
    
    mx_get_first_quote_indx(line, &ordinar, &doubl, &third);

    *quote_type = 'q';
    mx_get_quote_type(&ordinar, &doubl, &third, quote_type);
    if (*quote_type == 'q')
        return 0;
    else {
        ordinar = 0;
        for (int i = 0; i < mx_strlen(line); i++)
            if (line[i] == *quote_type && (i == 0 || line[i - 1] != '\\'))
                ordinar++;
        if (ordinar % 2 != 0)
            return -1;
        else
            return ordinar;
    }
}
