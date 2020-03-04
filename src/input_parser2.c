#include "ush.h"

void split_by_delimiter(char ***av);

t_lst *mx_ush_read_line(t_cmd_history **hist, t_global **hd) {
    char *line = NULL;
    char **av = NULL;
    t_lst *head = NULL;
    //t_lst *tmp = NULL;
    int i = 0;

    line = noncanon_read_line(hist);

    av = mx_ush_split_line(line, NULL);      /* розділяємо строку на токени  */

    while (av[i++] != NULL) {
        split_by_delimiter(&av);            /* ділимо кожну строку на підстроки */
    }                                       /* по крапці з комою */

    hd = NULL;
    free(line);
    free(av);
    exit(0);
    return head;
}

//кожну строку з av[] ділимо на підстроки по крапці з комою
//якщо перший аргумент - не команда, тоді шукаємо ; і відповідно нову команду
//інакше - записуємо перший аргумент як команду, а решту аргументів 
    //(до найближчої крапки з комою) - як аргументи цієї команди

//має вийти наступна конструкція: {ls} {-a} {-l} {src} {;} {cd} {inc} {;} {pwd}


char *write_before_delim(char *str, int j) {
    char *s = mx_strnew(j);
    bool flag = false;

    for(int i = 0; i < j; i++) {
        flag = true;
        s[i] = str[i];
    }
    if(flag)
        return s;
    else
        return "";
}

//функція повертає масив строк поділених по делімітеру у вигляді {ls} {;} {pwd} {NULL}
static char **split_to_substr(int *num_of_substr, char c, char *str) {
    char **str_arr = (char **)malloc(sizeof(char *) * 256);
    int substr = 0;

    str_arr[substr] = NULL;
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] == c) {
            str_arr[substr] = mx_strndup(str, i);
            if(mx_strlen(str_arr[substr]) == 0) {
                str_arr[substr] = mx_strdup(";");
                substr++;
            }
            else {
                substr++;
                str_arr[substr] = mx_strdup(";");
                substr++;
            }
            if(mx_get_char_index(&str[i + 1], ';') < 0) {
                str_arr[substr] = mx_strdup(&str[i + 1]);
                substr++;
                break;
            }
            else
                str[i] = '\0';
        }
    }
    *num_of_substr = substr;
    str_arr[substr] = NULL;

    for(int i = 0; i < substr; i++) {
        printf("#%i -> %s\n", i, str_arr[i]);
    }
    return str_arr;
}

void split_by_delimiter(char ***av) {
    int indx;
    int num_of_substr = 0;
    char **substrings = NULL;

    for(int i = 0; (*av)[i]; i++) {
        indx = mx_get_char_index((*av)[i], ';');
        substrings = split_to_substr(&num_of_substr, ';', (*av)[i]);
        // if(substrings != NULL) {    //якщо є ; - додаємо масив строк що повернула функція в основний масив аргументів
        //     for(int j = 0; substrings[j]; j++) {
        //         mx_printstr(substrings[j]);
        //         mx_printstr("\t");
        //     }
        //     printf("\namount = %i\n", num_of_substr);
        // }
        //якщо немає ; - пропускаємо 
    }
}

bool mx_is_command(char *str) {
    char **comands = get_commands();            //потрібно потім ЗАФРІШИТИ!!
    
    for(int i = 0; i < COMMANDS; i++) {
        if(strcmp(str, comands[i]) == 0) {
            free(comands);
            return true;
        }
    }
    free(comands);
    return false;
}

char **get_commands() {
    char **comands = (char **)malloc(sizeof(char *) * COMMANDS);

    comands[0] = "export";
    comands[1] = "unset";
    comands[2] = "fg";
    comands[3] = "exit";
    comands[4] = "env";
    comands[5] = "cd";
    comands[6] = "pwd";
    comands[7] = "which";
    comands[8] = "echo";
    comands[9] = "ls";
    return comands;
}


/*******************************************************************************************************************************************/
//////////////////////////////////////////////////////////////////ENV part///////////////////////////////////////////////////////////////////
/*******************************************************************************************************************************************/

//створює новий масив змінних із тих що ввів користувач (після флага -і) для передачі їх в EXECVE
char **fill_new_env(t_global **hd, int *i) {
    char **new_env = (char **)malloc(sizeof(char *));
    int j = 0;

    new_env[0] = NULL;

    for(*i = 2; mx_get_char_index((*hd)->new->av[*i], '=') > 0; (*i)++) { //формуємо список із нових аргументів доки зустрічаємо '=' в строці
        while(new_env[j] != NULL)
            j++;
        new_env = realloc(new_env, (sizeof(char *)) * (j + 2));
        new_env[j] = mx_strdup((*hd)->new->av[*i]);
        new_env[j + 1] = NULL;
    }
    return new_env;
}

///функція правильно розприділяє аргументи команди ENV для подальшої передачі в EXECVE///
void reparse_input_for_env(t_global **hd, int i, char *folder_to_search) {
    char *filename;
    char **buf = NULL;
    int j = 0;
    char *temp = NULL;

    filename = mx_strdup((*hd)->new->av[i]);
    buf = (char **)malloc(sizeof(char *) * BUFSIZE);
    buf[0] = NULL;
    while((*hd)->new->av[++i])
        buf[j++] = mx_strdup((*hd)->new->av[i]);                //зберігаємо флаги якщо вони є
    if((*hd)->new->cmd) {
        mx_del_strarr(&(*hd)->new->av);
        free((*hd)->new->cmd);
    }
    if(filename[0] != '/') {
        if(folder_to_search == NULL)                            //якщо флаг -Р не введено тоді бінарник шукаємо в /bin
            (*hd)->new->cmd = mx_strjoin("/bin/", filename);        //записуємо шляха до бінарника
        else {                                                  //якщо флаг -P активовано, тоді шукаємо у введеній директорії
            if(folder_to_search[strlen(folder_to_search) - 1] == '/') {
                (*hd)->new->cmd = mx_strjoin(folder_to_search, filename);
            }
            else {
                temp = mx_strjoin(folder_to_search, "/");
                (*hd)->new->cmd = mx_strjoin(temp, filename);
                free(temp);
            }
            free(folder_to_search);
        }
    }
    else
        (*hd)->new->cmd = mx_strdup(filename);
    (*hd)->new->av = (char **)malloc(sizeof(char *) * 2);   //формуємо масив аргументів
    (*hd)->new->av[0] = mx_strdup(filename);
    (*hd)->new->av[1] = NULL;

    for(j = 0; buf[j]; j++)
        add_new_arg((*hd)->new, buf[j]);
    mx_del_strarr(&buf);
}

//функція перевіряє чи менша строка входить в більшу (до знака = в більшій)
bool find_var_in_str(char *big, char *little) {
    int i = 0;
    bool res = true;

    if(big[0] == little[0]) {
        while(big[i] != '\0' && big[i] != '=') {
            if(big[i] != little[i]) {
                res = false;
                break;
            }
            i++;
        }
        if(res)
            return res;
    }
    return false;
}

//функція для реалізації -u (ігнорує записані користувачем змінні)
char **ignore_variables(t_global **hd) {
    int size = 0;
    char **new_env = NULL;
    int j = 0;

    while((*hd)->env[size])
        size++;
    new_env = (char **)malloc(sizeof(char *) * size + 1);
    for(int i = 0; i < size; i++) {
        if(!find_var_in_str((*hd)->env[i], (*hd)->new->av[2]))
            new_env[j++] = mx_strdup((*hd)->env[i]);
    }
    new_env[j] = NULL;
    return new_env;
}


////////////////окремі функції для флагів ENV/////////////////////////
char **i_flag_env(t_global **hd) {
    char **new_env;
    int i;
    char *folder_to_search = NULL;

    if(!(*hd)->new->av[2])                  //якщо немає аргументів після 'і' тоді нічого не робимо 
        return NULL;
    else {
        new_env = fill_new_env(hd, &i);     //заповнюємо новий масив із введених змінних
        if(mx_strcmp((*hd)->new->av[2], "-P") == 0) {
            if((*hd)->new->av[3]) {
                if(!(*hd)->new->av[4])    //якщо після -Р тільки один аргумент то виходимо
                    return NULL;            //just skiip and exit!!
                folder_to_search = mx_strdup((*hd)->new->av[3]);
                i = 4;
            }
            else {
                mx_usage_env("P");
                return NULL;
            }
        }
        if((*hd)->new->av[i]) {             //якщо далі є команда яку потрібно виконати тоді 
            reparse_input_for_env(hd, i, folder_to_search);   //передаємо їй новий масив і виконуємо цю команду в execve()
            return new_env;
        }
        else {                              //якщо команди немає, тоді просто виводимо
            mx_print_env(new_env);          //введені користувачем змінні
            mx_del_strarr(&new_env);
            return NULL;
        }
    }
}

char **u_flag_env(t_global **hd) {
    char **new_env;

    if(!(*hd)->new->av[2]) {         //якщо немає аргументів після 'u' тоді виводимо usage 
        mx_usage_env("u");
        return NULL;
    }
    else {
        new_env = ignore_variables(hd);    //копіюємо масив змінних оболонки
                                                // i видаляємо введені користувачем зміннні
        if(!(*hd)->new->av[3]) {            //якщо немає програми для запуску, тоді просто виводимо поточні змінні
            mx_print_env(new_env);          //ігноруючи задану користувачем
            return NULL;
        }
        else {
            reparse_input_for_env(hd, 3, NULL);       //якщо є програма для запуску, передаємо їй змінні, виключаючи задану користувачем
            return new_env;
        }
    }
}

char **p_flag_env(t_global **hd) {
    char **new_env;
    char *folder_to_search = NULL;

    if(!(*hd)->new->av[2]) {         //якщо немає аргументів після '-P' тоді виводимо usage 
        mx_usage_env("P");
        return NULL;
    }
    else {
        if(!(*hd)->new->av[3]) {
            mx_print_env((*hd)->env);
            return NULL;
        }
        else {
            new_env = mx_env_copy();
            folder_to_search = mx_strdup((*hd)->new->av[2]);
            reparse_input_for_env(hd, 3, folder_to_search);
            return new_env;
        }
    }
}


/////////////          ОСНОВНИЙ АЛГОРИТМ env         /////////////
char **mx_parse_env_args(t_global **hd) {
    char **new_env;

    if(!(*hd)->new->av[1]) {          //якщо env без флагів тоді просто виводимо змінні оболонки
        mx_print_env((*hd)->env);
        return NULL;
    }
    else if(mx_strcmp((*hd)->new->av[1], "-i") == 0) {
        new_env = i_flag_env(hd);
        if(new_env != NULL)
            return new_env;
        return NULL;
    }
    else if(mx_strcmp((*hd)->new->av[1], "-u") == 0) {
        new_env = u_flag_env(hd);
        if(new_env != NULL)
            return new_env;
        return NULL;
    }
    else if(mx_strcmp((*hd)->new->av[1], "-P") == 0){
        new_env = p_flag_env(hd);
        if(new_env != NULL)
            return new_env;
        return NULL;
    }
    else {
        reparse_input_for_env(hd, 1, NULL);
        new_env = mx_env_copy();
        return new_env;
    }
    return NULL;
}
