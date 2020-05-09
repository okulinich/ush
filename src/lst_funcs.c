#include "ush.h"

t_lst *mx_create_lst_node(char *str) {
    t_lst *node = (t_lst *)malloc(sizeof(t_lst));
    node->cmd = strdup(str);
    node->av = (char **)malloc(sizeof(char *) * 2);
    node->av[0] = strdup(str);
    node->av[1] = NULL;
    node->next = NULL;
    return node;
}

void mx_delete_list(t_lst *head) {
    t_lst *temp = head;

    while (head) {
        free(head->cmd);
        mx_del_strarr(&head->av);
        temp = head;
        head = head->next;
        free(temp);
    }
}

t_lst *mx_push_back_lst(t_lst **head, char *command) {
    t_lst *root = *head;

    if (!head || !(*head)) {
        *head = mx_create_lst_node(command);
        return *head;
    }
    else {
        while (root->next) {
            root = root->next;
        }
        root->next = mx_create_lst_node(command);
        return root->next;
    }
}

void mx_add_new_arg(t_lst *tmp, char *arg) {
    int i = 0;

    if (tmp->av[i] == NULL)
        tmp->av[i] = strdup(arg);
    else {
        while (tmp->av[i] != NULL)
            i++;
        tmp->av = realloc(tmp->av, (i + 2) * sizeof(char *));
        tmp->av[i] = strdup(arg);
        tmp->av[i + 1] = NULL;
    }
}

int mx_search_for_var_in_env(char *str) {
    extern char **environ;

    for (int i = 0; environ[i]; i++) {
        if (mx_find_var_in_str(environ[i], str)) {
            return i;
        }
    }
    return -1;
}
