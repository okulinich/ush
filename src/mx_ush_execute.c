#include "ush.h"

int find_add(t_global *hd, t_lst *head) {
    if (mx_strcmp(head->cmd, "help") == 0)
        return mx_help_command(hd, head);
    else if (mx_strcmp(head->cmd, "true") == 0)
        return mx_call_true(hd);
    else if (mx_strcmp(head->cmd, "false") == 0)
        return mx_call_false(hd);
    else if (mx_strcmp(head->cmd, "set") == 0)
        return mx_set(head);
    else if (mx_strcmp(head->cmd, "bye") == 0)
        return mx_bye(hd, head);
    // else if (mx_strcmp(head->cmd, "return") == 0)
        // return mx_return(hd, head);
    return 0;
}

int mx_find_builtin(t_global *hd, t_lst *head) {
    if (mx_strcmp(head->cmd, "pwd") == 0)
        return mx_builtin_pwd(head); // ret 1
    else if (mx_strcmp(head->cmd, "exit") == 0)
        return mx_exit(hd, head); // ret 1
    else if (mx_strcmp(head->cmd, "env") == 0)
        return mx_ush_launch(hd, head); // ret 1
    else if(mx_strcmp(head->cmd, "echo") == 0)
        return mx_echo(hd, head);
    else if(mx_strcmp(head->cmd, "export") == 0)
        return mx_export(head);
    else if(mx_strcmp(head->cmd, "cd") == 0)
        return mx_cd(hd, head);
    else if(mx_strcmp(head->cmd, "unset") == 0)
        return mx_unset(head);
    else if(mx_strcmp(head->cmd, "which") == 0)
        return mx_which(head);
    // else if(mx_strcmp(head->cmd, "fg") == 0)
        // return mx_fg(hd, head);
    return find_add(hd, head);
}

static void execute_av_in_qoutes(t_global *hd, t_lst *head) {
    int indx = -1;

    indx = check_cmd_args_for_commands(head);
    while(indx > 0) {
        mx_repl_quotes_with_cmd(&head->av[indx], hd);
        indx = check_cmd_args_for_commands(head);
    }
}

static void cmd_sub_tilda(t_lst *head) {
    char *user_name = search_for_var("USER");
    char *tilda_value = mx_strjoin("/Users/", user_name);
    char *temp = NULL;

    for(int i = 1; head->av[i] != NULL; i++) {
        while(mx_get_char_index(head->av[i], '~') >= 0) {
            temp = mx_replace_substr(head->av[i], "~", tilda_value);
            free(head->av[i]);
            head->av[i] = mx_strnew(mx_strlen(temp));
            mx_strcpy(head->av[i], temp);
            free(temp);
        }
    }

    free(user_name);
    free(tilda_value);
}
//тут перевірка аргументів на наявність  `cmd` і запуск відповідної ф-кції
//результат віконання цієї ф-кції = строка з виводом запущеної команди
//ця строка записується в якості аргумента поміщеного в ``
int mx_ush_execute(t_global *hd, t_lst *head) {
    int ret = -2;

    if (head->cmd == NULL)
        return 1;
    execute_av_in_qoutes(hd, head); //getting output from command placed in `` (cmd subtitution)
    cmd_sub_tilda(head); //replacing ~ with Users/USER
    ret = mx_find_builtin(hd, head); // 0 launch - 1 builtin
    if (ret < 0) { // -1 error 
        hd->last_exit_status = 1;
        return 1;
    }
    else if (ret > 0) { // builtin
        hd->last_exit_status = 0;
        return 1;
    }
    else if (ret == 0)
        ret = mx_ush_launch(hd, head);
    return ret;
    // return mx_find_builtin(hd, head) ? 1 : mx_ush_launch(hd, head);
    // если нашло билтин то рет 1 иначе лаунч
}

