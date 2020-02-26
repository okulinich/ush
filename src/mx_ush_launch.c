#include "ush.h"

static int func_exec(t_global *hd) {
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

int mx_ush_launch(t_global *hd) {
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

