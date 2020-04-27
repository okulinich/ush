#include "ush.h"

int check_cmd_args_for_commands(t_lst *cur) {
	for(int i = 0; cur->av[i] != NULL; i++) {
		if(cur->av[i][0] == '`' && cur->av[i][mx_strlen(cur->av[i]) - 1] == '`')
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
    char *str = mx_strnew(255);


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
    
    head = parse_cmd_to_list(cmd);
    root = head;

    while(head) {
        pipe(spwn.fds);
        posix_spawn_file_actions_init(&spwn.actions);
        posix_spawn_file_actions_adddup2(&spwn.actions, spwn.fds[1], 1);
        spwn.status = posix_spawnp(&spwn.child, head->cmd, &spwn.actions, NULL, head->av, hd->env);
        
        if(spwn.status == 0) {
            spwn.status = waitpid(spwn.child, &spwn.status, 0);
            str = get_command_output(&spwn, hd);
        }
        else
            printf("Posix spawn error: %s", strerror(spwn.status));
        head = head->next;
    }

    delete_list(root);
    return str;
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
        for(int i = 0; av[i] != NULL; i++)
             catch_escape_seq(av[i]); 
        global = (char **)malloc(sizeof(char *) * BUFSIZE);
        for(i = 0; av[i] != NULL; i++) {
            if(av[i][0] == '\'' || av[i][0] == '\"')        //токен в лапках розглядаємо як суцільний аргумент
                global[gl_i++] = mx_strndup(&av[i][1], strlen(av[i]) - 2);
            else {
                temp = mx_ush_split_line(av[i], NULL);      //токен не в лапках ділимо на підстроки
                split_by_delimiter(&temp);
                for(int j = 0; temp[j] != NULL; j++)
                    global[gl_i++] = mx_strdup(temp[j]);
                mx_del_strarr(&temp);
            }
        }
        fill_cmd_list(global, &head);
        mx_del_strarr(&global);
    }
    else {
        //catch_escape_seq(line); //function that takes string and catches (ekrans) escape sequances
        av = mx_ush_split_line(line, NULL);      /* розділяємо строку на токени  */
        //split_by_delimiter(&av);                /* по крапці з комою */
        i = 0;
        fill_cmd_list(av, &head);
    }

    free(line);
    if(mx_strcmp(av[0], "ERROR") != 0)
        mx_del_strarr(&av);
    return head;
}
