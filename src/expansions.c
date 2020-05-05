#include "ush.h"

int check_cmd_args_for_commands(t_lst *cur) {
	int indx = -1;

    for(int i = 0; cur->av[i] != NULL; i++) {
        indx = mx_get_char_index(cur->av[i], '`');
		if(indx != -1 && mx_get_char_index(&cur->av[i][indx + 1], '`') != -1)
			return i;
	}
	return 0;
}

    //отримано строку - вона має спарситись як звичайна срока 
    //введена з терміналу і список команд має послідовно 
    //виконатись сформувавши одну строку
static t_lst *parse_cmd_to_list(char *cmd) {
    t_lst *head = NULL;
    char *str = mx_strdup(&cmd[1]);

    str[mx_strlen(str) - 1] = '\0';
    head = parse_string(str);
    free(str);
    return head;
}

static char *get_command_output(t_spawn_args *spwn, t_global *hd) {
    char *str = mx_strnew(1024);


    if(spwn->status != -1)
        hd->last_exit_status = spwn->status;
    else
        perror("waitpid");
    close(spwn->fds[1]);
    for(int i = 0; read(spwn->fds[0], &str[i], 1) > 0; i++)  
        ;
    str[mx_strlen(str) - 1] = '\0';
    close(spwn->fds[0]);
    return str;
}

char *get_cmd_output(char *cmd, t_global *hd) {
	t_spawn_args spwn;
    char *str = NULL;
    t_lst *head = NULL;
    t_lst *root = NULL;
    char *tmp = NULL;
    extern char **environ;

    head = parse_cmd_to_list(cmd);
    root = head;

    while(head) {
        if(mx_strcmp(head->cmd, "echo") == 0) {
            int indx = check_cmd_args_for_commands(head);
            if(indx > 0) {
                str = get_cmd_output(head->av[indx], hd);
                delete_list(root);
                return str;
            }
            if(head->av[1] && mx_strcmp(head->av[1], "-n") != 0) {
                str = mx_strnew(strlen(head->av[1]));
                mx_strcpy(str, head->av[1]);
                delete_list(root);
                return str;
            }
            else if(head->av[1] && mx_strcmp(head->av[1], "-n") == 0 && head->av[2]) {
                str = mx_strnew(strlen(head->av[2]));
                mx_strcpy(str, head->av[2]);
                delete_list(root);
                return str;
            }
            else {
                delete_list(root);
                return mx_strnew(1);
            }
        }

        pipe(spwn.fds);
        posix_spawn_file_actions_init(&spwn.actions);
        posix_spawn_file_actions_adddup2(&spwn.actions, spwn.fds[1], 1);
        spwn.status = posix_spawnp(&spwn.child, head->cmd, &spwn.actions, NULL, head->av, environ);
        
        if(spwn.status == 0) {
            spwn.status = waitpid(spwn.child, &spwn.status, 0);
            tmp = get_command_output(&spwn, hd);
            if(str == NULL) {
                str = mx_strdup(tmp);
                free(tmp);
            }
            else {
                char *buf = mx_strjoin(str, tmp);
                free(tmp);
                free(str);
                str = buf;
            }
        }
        else {
            mx_printstr("Posix spawn error: "); 
            mx_printstr(strerror(spwn.status));
        }
        posix_spawn_file_actions_destroy(&spwn.actions);
        head = head->next;
    }

    delete_list(root);
    return str;
}




void mx_repl_quotes_with_cmd(char **cmd, t_global *hd) {
    char *command = NULL;
    char *new_av = NULL;
    char *output = NULL;
    int indx = 0;
    int end_indx = 0;
    int j = 0;

    indx = mx_get_char_index(*cmd, '`');
    while(indx != -1) {
        command = mx_strnew(BUFSIZE);
        new_av = mx_strnew(BUFSIZE);
        //getting substr in `` from str 
        for(int i = indx; i < mx_strlen(*cmd); i++) {
            command[j++] = (*cmd)[i];
            if((*cmd)[i] == '`' && i != indx) {
                end_indx = i;
                break;
            }
        }

        output = get_cmd_output(command, hd);

        if(output == NULL) {
            output = mx_strnew(1);
        }

        //creating new str and replacing `cmd` with cmd output
        //1 - copying original str before first `
        mx_strncpy(new_av, *cmd, indx);
        j = indx;
        //2 - copying output of cmd 
        for(int i = 0; i < mx_strlen(output); i++) {
            new_av[j++] = output[i];
            if(j >= (int)malloc_size(new_av))
                new_av = (char *)realloc(new_av, malloc_size(new_av) + BUFSIZE);
        }
        //3 - copying least original str after second `
        mx_strcpy(&new_av[j], &(*cmd)[end_indx + 1]);


        free(*cmd);
        *cmd = mx_strdup(new_av);

        free(new_av);
        free(command);
        free(output);
        indx = end_indx = j = 0;

        indx = mx_get_char_index(*cmd, '`');

    }
}


bool mx_replace_pharent_with_quotes(char *line) {
    int brackets = 0;
    bool replace_with_quotes = false;

    for(int i = 0; i < mx_strlen(line); i++) {
        if(line[i] == '(') {
            brackets++;
            if(i - 1 >= 0 && line[i - 1] == '$') {
                replace_with_quotes = true;
                line[i - 1] = '`';
                for(int j = i; j < mx_strlen(line); j++)
                    line[j] = line[j + 1];
            }
        }
        else if(line[i] == ')') {
            brackets--;
            if(replace_with_quotes) {
                replace_with_quotes = false;
                line[i] = '`';
            }
        }
        else if(brackets > 0 && line[i] == '\"')
            line[i] = '\'';
    }

    if(brackets != 0) {
        mx_printerr("ush: ERROR: odd number of pharenthesis\n");
        line[0] = '\0';
        return false;
    }
    
    return true;
}


t_lst *parse_string(char *str) {
    char *line = mx_strdup(str);
    char **av = NULL;
    char **global;
    int gl_i = 0;
    char **temp;
    t_lst *head = NULL;
    int i = 0;

    av = mx_split_by_quotes(line);              //розбиваємо стркоу по лапках

    if(av && mx_strcmp(av[0], "ERROR") == 0) {  //якщо лапки не закриті - видаємо помилку
        mx_printerr("ush: ERROR: Odd number of quotes.\n");
        free(av);
    }
    else if(av) {
        global = (char **)malloc(sizeof(char *) * BUFSIZE);
        for(i = 0; av[i] != NULL; i++) {
            if(av[i][0] == '\'' || av[i][0] == '\"')        //токен в лапках розглядаємо як суцільний аргумент
                global[gl_i++] = mx_strndup(&av[i][1], strlen(av[i]) - 2);
            else if(av[i][0] == '`')
                global[gl_i++] = mx_strdup(av[i]);
            else {
                temp = mx_ush_split_line(av[i], NULL);      //токен не в лапках ділимо на підстроки
                split_by_delimiter(&temp);
                for(int j = 0; temp[j] != NULL; j++)
                    global[gl_i++] = mx_strdup(temp[j]);
                mx_del_strarr(&temp);
            }
        }
        global[gl_i] = NULL;
        fill_cmd_list(global, &head);
        mx_del_strarr(&global);
    }
    else {
        av = mx_ush_split_line(line, NULL);      /* розділяємо строку на токени  */
        split_by_delimiter(&av);                /* по крапці з комою */
        i = 0;
        fill_cmd_list(av, &head);
    }

    free(line);
    if(av && av[0] && mx_strcmp(av[0], "ERROR") != 0)
        mx_del_strarr(&av);
    return head;
}
