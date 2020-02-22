#include "ush.h"

t_lst *create_node(char *str) {
    t_lst *node = (t_lst *)malloc(sizeof(t_lst));
    node->cmd = strdup(str);
    node->av = (char **)malloc(sizeof(char *) * 2);
    node->av[0] = strdup(str);
    node->av[1] = NULL;
    node->next = NULL;
    return node;
}

t_lst *push_back(t_lst **head, char *command) {
    t_lst *root = *head;

    if(!head || !(*head)) {
        *head = create_node(command);
        return *head;
    }
    else {
        while(root->next) {
            root = root->next;
        }
        root->next = create_node(command);
        return root->next;
    }
}

void add_new_arg(t_lst *tmp, char *arg) {
    int i = 0;

    if(tmp->av[i] == NULL)
        tmp->av[i] = strdup(arg);
    else {
        while(tmp->av[i] != NULL)
            i++;
        tmp->av = realloc(tmp->av, (i + 2) * sizeof(char *));
        tmp->av[i] = strdup(arg);
        tmp->av[i + 1] = NULL;
    }
}
