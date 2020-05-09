#include "ush.h"

int main() {
    t_global *head = malloc(sizeof(t_global));
    char *line;
    size_t buf = 0;
    
    head->input = NULL;
    head->last_exit_status = 0; 
    mx_nosig();    
    if(isatty(0) == 0) {
        if(getline(&line, &buf, stdin) < 0) {
            exit(1);
        }
        head->input = mx_strnew(1024);
        mx_strcpy(head->input, line);
    }
    else {
        head->input = mx_strnew(1024);
        mx_strcpy(head->input, "emptyinput");
    }
    mx_set_correct_shlvl();
    mx_ush_loop(head);
}

void mx_set_correct_shlvl(void) {
    char *value = mx_ssearch_for_var_in_env("SHLVL");
    int i_value = 0;
    char *new_value = NULL;

    if  (value != NULL) {
        i_value = atoi(&value[1]);
        new_value = mx_itoa(i_value + 1);
        setenv("SHLVL", new_value, 1);
        free(new_value);
        free(value);
    }
}
