#include "ush.h"

//getting substr in `` from str 
static void get_full_command(char *command, t_index *indx, char **cmd) {
    for (int i = indx->indx; i < mx_strlen(*cmd); i++) {
        command[indx->j++] = (*cmd)[i];
        if ((*cmd)[i] == '`' && i != indx->indx 
            && (i == 0 || (*cmd)[i - 1] != '\\')) {
            indx->end_indx = i;
            break;
        }
    }
}

static void init_index(t_index *indx) {
    indx->indx = 0;
    indx->end_indx = 0;
    indx->j = 0;
} 

static void copy_orig_part_and_cmd_output(char **new_av, char **cmd, 
                                            char **output, t_index *indx) {
    if (*output == NULL)
        *output = mx_strnew(1);
    //creating new str and replacing `cmd` with cmd output
    //1 - copying original str before first `
    mx_strncpy(*new_av, *cmd, indx->indx);
    indx->j = indx->indx;
    //2 - copying output of cmd 
    for (int i = 0; i < mx_strlen(*output); i++) {
        (*new_av)[indx->j++] = (*output)[i];
        if (indx->j >= (int)malloc_size(*new_av))
            *new_av = (char *)realloc(*new_av, malloc_size(*new_av) + MX_BUFSIZE);
    }
    //3 - copying least original str after second `
    mx_strcpy(&(*new_av)[indx->j], &(*cmd)[indx->end_indx + 1]);
    free(*cmd);
    *cmd = mx_strdup(*new_av);
}

void mx_repl_quotes_with_cmd(char **cmd, t_global *hd) {
    char *command = NULL;
    char *new_av = NULL;
    char *output = NULL;
    t_index indx;

    init_index(&indx);
    indx.indx = mx_get_char_index(*cmd, '`');
    while (indx.indx != -1) {
        command = mx_strnew(MX_BUFSIZE);
        new_av = mx_strnew(MX_BUFSIZE);
        get_full_command(command, &indx, cmd);
        output = mx_get_cmd_output(command, hd);
        copy_orig_part_and_cmd_output(&new_av, cmd, &output, &indx);
        //go futher along the line
        free(new_av);
        free(command);
        free(output);
        init_index(&indx);
        indx.indx = mx_get_char_index(*cmd, '`');
    }
}
