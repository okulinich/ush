#include "ush.h"

//check wether semicolons are placed correctly (bash-like style)
bool mx_semicol_placed_right(char **global, t_lst **head, int j) {
    if (j == 0) {
        mx_printerr("ush: syntax error near unexpected token `;\'\n");
        *head = NULL;
        return false;
    }
    else if (mx_strcmp(global[j - 1], ";") == 0) {
        mx_delete_list(*head);
        *head = NULL;
        mx_printerr("ush: syntax error near unexpected token `;;\'\n");
        return false;
    }
    return true;
}

//checks if all types of qoutes are used correctly
bool mx_parse_quotes(char *line, char ***av) {
    if (!mx_replace_pharent_with_quotes(line)) {
        free(line);
        return false;
    }
    *av = mx_split_by_quotes(line); //розбиваємо стркоу по лапках
    if (*av && mx_strcmp((*av)[0], "ERROR") == 0) {  
        mx_printerr("ush: ERROR: Odd number of quotes.\n");
        free(*av);
        free(line);
        return false;
    }
    return true;
}

static char **copy_av(char **av) {
    char **new_av = NULL;
    int size;

    for (size = 0; av[size] != NULL; size++)
        ;
    new_av = (char **)malloc(sizeof(char *) * size + 1);
    for (int i = 0; i < size; i++)
        new_av[i] = mx_strdup(av[i]);
    new_av[size] = NULL;
    return new_av;
}

//
void mx_parse_str_with_quotes(char ***global, char ***av) {
    int gl_i = 0;
    char **temp = NULL;
    char **av_copy = copy_av(*av);
 
    *global = (char **)malloc(sizeof(char *) * MX_BUFSIZE);
    for (int i = 0; (*av)[i] != NULL; i++) {
        if (i == 0 || mx_get_substr_index(av_copy[i - 1], "echo") < 0)
            mx_catch_escape_seq((*av)[i]);
        if ((*av)[i][0] == '\'' || (*av)[i][0] == '\"')    
            (*global)[gl_i++] = mx_strndup(&(*av)[i][1], strlen((*av)[i]) - 2);
        else if ((*av)[i][0] == '`')
            (*global)[gl_i++] = mx_strdup((*av)[i]);
        else {
            temp = mx_ush_split_line((*av)[i], NULL);      
            mx_split_by_delimiter(&temp);
            mx_add_tokens_to_arr(global, &temp, &gl_i);
        }
    }
    (*global)[gl_i] = NULL;
    mx_del_strarr(&av_copy);
}

void mx_free_used_memory(char *line, char **av) {
    if (mx_strlen(line) > 0 && malloc_size(line) > 0)
        free(line);
    if (av && av[0] != NULL && mx_strcmp(av[0], "ERROR") != 0) {
        mx_del_strarr(&av);
    }
}
