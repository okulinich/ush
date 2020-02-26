/*Цикл життя командної оболонки:
    - ініціалізація: шел читає і виконує свої файли конфігурацій
    - інтерпритація: шел зчитує команди із stdin і виконує їх
    - завершення: фрішемо пам'ять і завершуємо роботу
*/

/*
    Базовий цикл Шела:
    - зчитування команд із стандартних потоків
    - парсинг строки
    - виконання (команди)
*/

#include "ush.h"

////////////////////////////////////парсинг строки//////////////////////////////////////
//функція повертає масив слів із зчитаної строки
char **ush_split_line(char *line) {
    int buf_size = BUFSIZE;
    int pos = 0;
    char **tokens = (char **)malloc(buf_size * sizeof(char *));       //масив слів із строки
    char *token;

    token = strtok(line, DELIMITERS);
    while(token) {
        tokens[pos] = token;                                  //записуємо кожне слово в масив
        pos++;

        if(pos >= buf_size) {                           //розширюємо масив слів якшо потрібно
            buf_size += BUFSIZE;
            tokens = realloc(tokens, buf_size * (sizeof(char *)));
        }
        token = strtok(NULL, DELIMITERS);
    }
    tokens[pos] = NULL;
    return tokens;
}
/////////////////////////////////////////////////////////////////////////////////////////

int func_exec(t_global *hd) {
    int status = 0;
    char **new_env;

    if (mx_strcmp(hd->new->cmd, "env") == 0) {  
        new_env = mx_parse_env_args(&hd);       //функція повертає NULL якщо в env не передано ніяких команд
        if(new_env == NULL)                     //інакше - повертає масив змінних среди
            return 1;
        else
            status = execve(hd->new->cmd, hd->new->av, new_env); //запускаємо env з заданим набором змінних
        mx_del_strarr(&new_env);
    }
    else
        status = execv(hd->new->cmd, hd->new->av); // юзаємо с новими аргументами середи  
    return status;
}
//////////////////////////////////запуск програми//////////////////////////////////////
int ush_launch(t_global *hd) {
    pid_t pid;          //proccess id
    pid_t wpid;         //
    int status;
    int res = 0;

    pid = fork();       //створюємо процес і зберігаємо значення
//як тільки fork() повертає значення, ми отримуємо два паралельних процеса
//fork повертає нуль для child proccess і id процесу для батьківського процесу
    if(pid == 0) {      //нуль повертається child proccess
        //дочерний процес
        /*
            exec() - системний виклик, який дозволяє виконати вбудувані команди
            оболонки шел поза процесом самого шел
            v -> ми передаємо вектор (масив строк)
            p -> замість повного шляху до команди ми передаємо тільки її імя
        */
        res = func_exec(hd);
        if(res == 0) { // сделать так что бы выдавало ошибку command not found.
            perror("u$h");
            exit(1);
        }
        else if(res == 1)
            exit(0);
    }
    // if(pid < 0)
        // error
    wpid = waitpid(pid, &status, WUNTRACED);
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////

int ush_execute(t_global *hd) {
    if(hd->new->cmd == NULL)
        return 1;
    if (mx_strcmp(hd->new->cmd, "pwd") == 0)
        return mx_builtin_pwd(hd->new);
    if (mx_strcmp(hd->new->cmd, "exit") == 0) {
        //system("leaks -q ush");
        mx_exit(hd->new);
    }
        // функція яка знаходить білтін і юзає його
        // якщо енв то заходить ush_launch 
    
    return ush_launch(hd);
}

//////////////////////////////////////основний цикл/////////////////////////////////////
void ush_loop(t_global *hd) {
    t_lst *root;
    int status = 1;
    t_cmd_history *hist = NULL;

    while (status) {
        hd->new = lsh_read_line(&hist);                     //зчитуємо строку
        root = hd->new;
        for ( ; hd->new; hd->new = hd->new->next) {
            status = ush_execute(hd);                 //виконуємо команди
        }
        delete_list(root);
    }
    delete_history(hist);
}
/////////////////////////////////////////////////////////////////////////////////////////


int main() {
    t_global *head = malloc(sizeof(t_global *));

    head->env = mx_env_copy();
    ush_loop(head);         //цикл команд
    // free(a);
}
