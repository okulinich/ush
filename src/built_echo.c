#include "ush.h"

bool check_three_flag_combinations(t_lst *head, int *i, int j, 
                                    bool *no_new_line) {
    if (mx_strcmp(head->av[j], "-n") == 0 
        || mx_strcmp(head->av[j], "-enE") == 0 
        || mx_strcmp(head->av[j], "-Ene") == 0 
        || mx_strcmp(head->av[j], "-nEe") == 0 
        || mx_strcmp(head->av[j], "-neE") == 0 
        || mx_strcmp(head->av[j], "-Een") == 0 
        || mx_strcmp(head->av[j], "-eEn") == 0 
        || mx_strcmp(head->av[j], "-en")  == 0) {
        *no_new_line = true;
        (*i)++;
        return true;
    }
    return false;
}

bool check_e_flag_combinations(t_lst *head, int j, bool *escape_off, bool *no_new_line) {
    if (mx_strcmp(head->av[j], "-E") == 0) {
        *escape_off = true;
        return true;
    }
    else if (mx_strcmp(head->av[j], "-e") == 0 
            || mx_strcmp(head->av[j], "-eE") == 0 
            || mx_strcmp(head->av[j], "-Ee") == 0) {
        *escape_off = false;
        return true;
    }
    else if (mx_strcmp(head->av[j], "-nE") == 0 
            || mx_strcmp(head->av[j], "-En") == 0) {
        *no_new_line = true;
        *escape_off = true;
        return true;
    }
    return false;
}

static void parse_flags(bool *no_new_line, bool *escape_off, t_lst *head, int *i) {
    bool flag = true;

    for(int j = 1; head->av[j]; j++)
        if (head->av[j][0] == '-' && flag) {
            if (check_three_flag_combinations(head, i, j, no_new_line)) 
                continue;
            else if (check_e_flag_combinations(head, j, escape_off, no_new_line)) 
                (*i)++;
            else if (mx_strcmp(head->av[j], "-ne") == 0 
                    || mx_strcmp(head->av[j], "-en") == 0) {
                *no_new_line = true;
                *escape_off = false;
                (*i)++;
            }
        }
        else
            flag = false;
}

void mx_print_arguments(t_lst *head, int i, t_global *hd, bool escape_off) {
    for (; head->av[i]; i++) {
        if (mx_strcmp(head->av[i], "$?") == 0) {
            mx_printint(hd->last_exit_status);
        }
        else if (get_var_from_str(&head->av[i])) {
            while (get_var_from_str(&head->av[i]))
                ;
        }
        if (mx_strcmp(head->av[i], "$?") != 0)
            escape_off == true ? mx_printstr(head->av[i]) : 
                                 print_with_escape(head->av[i]);
        if (head->av[i + 1])
            mx_printstr(" ");
    }
}

int mx_echo(t_global *hd, t_lst *head) {
    bool no_new_line = false;
    bool escape_off = true;
    int i = 1;

    if (!head->av[1])
        mx_printstr("\n");
    else {
        parse_flags(&no_new_line, &escape_off, head, &i);
        mx_print_arguments(head, i, hd, escape_off);
        if (!no_new_line)
            mx_printstr("\n");
    }
    return 1;
}
