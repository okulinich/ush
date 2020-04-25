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

void delete_list(t_lst *head) {
    t_lst *temp = head;
    int i = 0;

    while (head) {
        free(head->cmd);
        mx_del_strarr(&head->av);
        //while(head->av[++i])
           // free(head->av[i]);//
        //free(head->av);
        temp = head;
        head = head->next;
        free(temp);
    }
    //free(temp);
}

void delete_history(t_cmd_history *head) {
    t_cmd_history *temp;

    while(head) {
        free(head->user_input);
        temp = head;
        head = head->next;
        free(temp);
    }
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

void push_front_history(t_cmd_history **head, char *line) {
    t_cmd_history *new_item = (t_cmd_history *)malloc(sizeof (t_cmd_history));

    new_item->user_input = mx_strdup(line);
    new_item->next = *head;
    new_item->prev = NULL;
    if(*head)
        (*head)->prev = new_item;
    *head = new_item;
}

char **init_vars() {
    char **str = (char **)malloc(sizeof(char *));
    str[1] = NULL;
    return str;
}

void delete_global(t_global *head) {
    if(head) {
        for(int i = 0; head->vars[i]; i++) {
            free(head->vars[i]);
        }
        free(head->vars);
        free(head);
    }
}

int search_for_var_in_vars(t_global *hd, char *str) {
    for(int i = 0; hd->vars[i]; i++) {
        if(find_var_in_str(hd->vars[i], str)) {
            return i;
        }
    }
    return -1;
}

int search_for_var_in_env(t_global *hd, char *str) {
    for(int i = 0; hd->env[i]; i++) {
        if(find_var_in_str(hd->env[i], str)) {
            return i;
        }
    }
    return -1;
}

void add_local_var(t_global **hd, char *str) {
    int i = 0;
    char *var_name = mx_strndup(str, mx_get_char_index(str, '='));
    //printf("\nInputed str = %s\nvar name = %s\n", str, var_name);
    
    i = search_for_var_in_vars(*hd, var_name);
    if(i != -1) {        //якщо змінну знайдено - заміняємо
        free((*hd)->vars[i]);
        (*hd)->vars[i] = mx_strdup(str);
    }
    i = search_for_var_in_env(*hd, var_name);
    if(i != -1) {
        free((*hd)->env[i]);
        (*hd)->env[i] = mx_strdup(str);
    }
    else {
        i = 0;
        while((*hd)->vars[i])
            i++;
        (*hd)->vars = realloc((*hd)->vars, (i + 2) * sizeof(char *));
        (*hd)->vars[i] = mx_strdup(str);
        (*hd)->vars[i + 1] = NULL;
    }
    free(var_name);
}
