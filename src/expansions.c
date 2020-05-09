#include "ush.h"

int mx_check_cmd_args_for_commands(t_lst *cur) {
	int indx = -1;

    for (int i = 0; cur->av[i] != NULL; i++) {
        indx = mx_get_char_index(cur->av[i], '`');
		if (indx != -1 && mx_get_char_index(&cur->av[i][indx + 1], '`') != -1)
			return i;
	}
	return 0;
}

//отримано строку - вона має спарситись як звичайна срока 
//введена з терміналу і список команд має послідовно 
//виконатись сформувавши одну строку
t_lst *mx_parse_cmd_to_list(char *cmd) {
    t_lst *head = NULL;
    char *str = NULL;
    
    if (cmd[0] == '\\') {
        str = mx_strdup(&cmd[2]);
        str[mx_strlen(str) - 2] = '\0';
    }
    else {
        str = mx_strdup(&cmd[1]);
        str[mx_strlen(str) - 1] = '\0';
    }
    if (str != NULL) {
        head = mx_parse_string(str);
        if (malloc_size(str) > 0)
            free(str);
    }
    return head;
}


static void find_and_replace_brackets(char *line, int *brackets, 
                                bool *replace_with_quotes, int i) {
    if (line[i] == '(') {
        (*brackets)++;
        if (i - 1 >= 0 && line[i - 1] == '$') {
            *replace_with_quotes = true;
            line[i - 1] = '`';
            for(int j = i; j < mx_strlen(line); j++)
                line[j] = line[j + 1];
        }
    }
    else if (line[i] == ')') {
        (*brackets)--;
        if (*replace_with_quotes) {
            *replace_with_quotes = false;
            line[i] = '`';
        }
    }
    else if (*brackets > 0 && line[i] == '\"')
        line[i] = '\'';
}

bool mx_replace_pharent_with_quotes(char *line) {
    int brackets = 0;
    bool replace_with_quotes = false;

    for (int i = 0; i < mx_strlen(line); i++)
        find_and_replace_brackets(line, &brackets, &replace_with_quotes, i);

    if (brackets != 0) {
        mx_printerr("ush: ERROR: odd number of pharenthesis\n");
        line[0] = '\0';
        return false;
    }
    
    return true;
}

t_lst *mx_parse_string(char *str) {
    char *line = mx_strdup(str);
    char **av = NULL;
    char **global;
    t_lst *head = NULL;

    if (!mx_parse_quotes(line, &av))
        return head;
    else if (av) {
        mx_parse_str_with_quotes(&global, &av);
        mx_fill_cmd_list(global, &head);
        mx_del_strarr(&global);
    }
    else {
        mx_catch_escape_seq(line); 
        mx_escape_seq_and_split_by_semicol(line, &av);
        mx_fill_cmd_list(av, &head);
    }
    mx_free_used_memory(line, av);
    return head;
}
