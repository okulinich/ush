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


//////////////////////////////////запуск програми//////////////////////////////////////
int ush_launch(t_lst *head) {
    pid_t pid;          //proccess id
    pid_t wpid;         //
    int status;

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
        if(execvp(head->cmd, head->av) < 0) { // сделать так что бы выдавало ошибку command not found.
            perror("u$h");
            exit(1);
        }
    }
    wpid = waitpid(pid, &status, WUNTRACED);
    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////

int ush_execute(t_lst *head) {
    if(head->cmd == NULL)
        return 1;
    if (mx_strcmp(head->cmd, "pwd") == 0)
        return mx_builtin_pwd(head);
    if (mx_strcmp(head->cmd, "exit") == 0)
        mx_exit(head);
    return ush_launch(head);
}

//////////////////////////////////////основний цикл/////////////////////////////////////
void ush_loop() {
    t_lst *head;
    t_lst *root;
    int status = 1;
    t_cmd_history *hist = NULL;

    while (status) {
        head = lsh_read_line(&hist);                     //зчитуємо строку
        root = head;
        for ( ; root; root = root->next) {
            status = ush_execute(root);                 //виконуємо команди
        }
        delete_list(head);
    }
    delete_history(hist);
}
/////////////////////////////////////////////////////////////////////////////////////////

// int main() {
//     ush_loop();         //цикл команд
//     system("leaks -q ush");
// }

int main() {
    // t_global *head = malloc(sizeof(t_global *));
    // head->env = mx_env_copy();
    // mx_print_strarr(head->env, "\n");
    // mx_del_strarr(&head->env);
    // system("leaks -q ush");
    // return 0;
    ush_loop();         //цикл команд
    // free(a);
}
