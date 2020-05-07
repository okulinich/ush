#include "ush.h"

// char **init_vars() { ne yuzaem vrode!!!!!!!!!1
//     char **str = (char **)malloc(sizeof(char *));

//     str[1] = NULL;
//     return str;
// }

// void delete_global(t_global *head) { ne yuzaem vrode!!!!!!!!!1
//     if(head) {
//         for(int i = 0; head->vars[i]; i++) {
//             free(head->vars[i]);
//         }
//         free(head->vars);
//         free(head);
//     }
// }

// int search_for_var_in_vars(t_global *hd, char *str) {
//     for(int i = 0; hd->vars[i]; i++) {
//         if(find_var_in_str(hd->vars[i], str)) {
//             return i;
//         }
//     }
//     return -1;
// }

int search_for_var_in_env(char *str) {
    extern char **environ;

    for(int i = 0; environ[i]; i++) {
        if(find_var_in_str(environ[i], str)) {
            return i;
        }
    }
    return -1;
}

// void add_local_var(t_global **hd, char *str) {
//     int i = 0;
//     char *var_name = mx_strndup(str, mx_get_char_index(str, '='));
    
//     i = search_for_var_in_vars(*hd, var_name);
//     if(i != -1) {        //якщо змінну знайдено - заміняємо
//         free((*hd)->vars[i]);
//         (*hd)->vars[i] = mx_strdup(str);
//     }
//     i = search_for_var_in_env(var_name);
//     if(i != -1) {
//         free((*hd)->env[i]);
//         (*hd)->env[i] = mx_strdup(str);
//     }
//     else {
//         i = 0;
//         while((*hd)->vars[i])
//             i++;
//         (*hd)->vars = realloc((*hd)->vars, (i + 2) * sizeof(char *));
//         (*hd)->vars[i] = mx_strdup(str);
//         (*hd)->vars[i + 1] = NULL;
//     }
//     free(var_name);
// }
