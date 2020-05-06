#include "ush.h"

//takes array of tokens readed from stdin and fills the list of commands
//that will be executed
void fill_cmd_list(char **global, t_lst **head) {
    bool semicolon = true;
    t_lst *tmp = *head;

    for (int j = 0; global[j]; j++) {
        if (mx_strcmp(global[j], "") == 0)
            continue;
        else if (mx_strcmp(global[j], ";") == 0) {     
            if (!semicol_placed_right(global, head, j))
                return ;
            semicolon = true;
            continue;
        }
        else
            if (semicolon) {
                tmp = push_back(head, global[j]);
                semicolon = false;
            }
            else
                add_new_arg(tmp, global[j]);
    }
}

//reads line from stdin or takes string already placed to stdin by pipe 
//from another proccess
void mx_read_line_from_stdin(char **line, char *input) {
    if(mx_strcmp("emptyinput", input) != 0) {
        *line = mx_strnew(1024);
        strcpy(*line, input);
    }
    else {
        *line = mx_noncanon_read_line();
        while(!mx_string_has_chars(*line) && mx_strlen(*line) > 0) {
            free(*line);
            *line = mx_noncanon_read_line();
        }
    }
}

// - ділимо строку по лапках
// - у випадку якщо лапки не закриті - видаємо помилку
// - токени які не були заключені в лапки - ділимо по спейс-симловах
// - а потім ділимо по крапці з комою
t_lst *mx_ush_read_line(char *input) {
    char *line = NULL;
    char **av = NULL;
    char **global;
    t_lst *head = NULL;

    mx_read_line_from_stdin(&line, input);
    if (!mx_parse_quotes(line, &av))
        return head;
    else if (av) {
        mx_parse_str_with_quotes(&global, &av);
        fill_cmd_list(global, &head);
        mx_del_strarr(&global);
    }
    else {
        catch_escape_seq(line); //function that takes string and catches (ekrans) escape sequances
        mx_escape_seq_and_split_by_semicol(line, &av);
        fill_cmd_list(av, &head);
    }
    mx_free_used_memory(line, av);
    return head;
}

