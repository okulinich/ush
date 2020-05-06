#include "ush.h"

//check wether semicolons are placed correctly (bash-like style)
bool semicol_placed_right(char **global, t_lst **head, int j) {
    if (j == 0) {
        mx_printerr("ush: syntax error near unexpected token `;\'\n");
        *head = NULL;
        return false;
    }
    else if (mx_strcmp(global[j - 1], ";") == 0) {
        delete_list(*head);
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
    *av = mx_split_by_quotes(line);              //розбиваємо стркоу по лапках
    if (*av && mx_strcmp((*av)[0], "ERROR") == 0) {  //якщо лапки не закриті - видаємо помилку
        mx_printerr("ush: ERROR: Odd number of quotes.\n");
        free(*av);
        free(line);
    }
    return true;
}

//
void mx_parse_str_with_quotes(char ***global, char ***av) {
    int gl_i = 0;
    char **temp = NULL;
    
    *global = (char **)malloc(sizeof(char *) * BUFSIZE);
    for (int i = 0; (*av)[i] != NULL; i++) {
        if (i == 0 || strstr((*av)[i - 1], "echo") == NULL)
            catch_escape_seq((*av)[i]);
        if ((*av)[i][0] == '\'' || (*av)[i][0] == '\"')        //токен в лапках розглядаємо як суцільний аргумент
            (*global)[gl_i++] = mx_strndup(&(*av)[i][1], strlen((*av)[i]) - 2);
        else if ((*av)[i][0] == '`')
            (*global)[gl_i++] = mx_strdup((*av)[i]);
        else {
            temp = mx_ush_split_line((*av)[i], NULL);      //токен не в лапках ділимо на підстроки
            split_by_delimiter(&temp);
            for (int j = 0; temp[j] != NULL; j++)
                (*global)[gl_i++] = mx_strdup(temp[j]);
            mx_del_strarr(&temp);
        }
    }
    (*global)[gl_i] = NULL;
}

void mx_free_used_memory(char *line, char **av) {
    if (mx_strlen(line) > 0)
        free(line);
    if (av && av[0] != NULL && mx_strcmp(av[0], "ERROR") != 0) {
        mx_del_strarr(&av);
    }
}

void mx_escape_seq_and_split_by_semicol(char *line, char ***av) {
    if (*av)
        mx_del_strarr(av);
    *av = mx_ush_split_line(line, NULL);      /* розділяємо строку на токени  */    
    split_by_delimiter(av);                /* по крапці з комою */
}
