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

//builtin commands: export, unset, fg, exit
char *builtin_str[] = {"cd", "help", "exit", "env"}; 

int ush_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int ush_cd(char **args) {
    if(args[1] == NULL) {
        fprintf(stderr, "ush: waiting argument for \"cd\"");
    }
    else {
        if(chdir(args[1]) != 0)     //встановлює в якості поочного каталог 
            perror("cd");      //на який вказує аргумент
    }
    return 1;
}

int ush_help(char **args) {
    printf("-------------USH-------------\n");
    printf("- List of builtin commands: -");
    for(int i = 0; i < ush_num_builtins(); i++)
        printf("- %s *\n", builtin_str[i]);
    return 1;
}

int ush_exit(char **args) {
    return 0;
}

void ush_env() {

}

int (*builtin_func[]) (char **args) = { &ush_cd, &ush_help, &ush_exit, &ush_env};


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
        //printf("********%s\n", head->cmd);
        if(execvp(head->cmd, head->av) == -1) {
            fprintf(stderr, "%s", head->cmd);
            exit(1);
        }
    }
    else if(pid < 0) {
            perror("ush: ");
    }
    else {
        //родительский процес
        do {
            //знаємо що дочерний процес має шось виконати, тому очікуємо на зміну стану цього процесу
            wpid = waitpid(pid, &status, WUNTRACED);        
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
//////////////////////////////////////////////////////////////////////////////////////


int ush_execute(t_lst *head) {
    int i;

    if(head->cmd == NULL) {
        return 1;
    }

    //цикл перевіряє чи є команда вбудованою
    //якшо так, тоді викликає її
    //якшо ні, то запускає процес через ush_launch
    for(i = 0; i < ush_num_builtins(); i++) {
        if(strcmp(head->cmd, builtin_str[i]) == 0)
            return (*builtin_func[i])(head->av);
    }
    return ush_launch(head);
}



//////////////////////////////////////основний цикл/////////////////////////////////////
void ush_loop() {
    char *line;
    t_lst *head;
    t_lst *root;
    int status;

    do {
        printf("u$h> ");
        head = lsh_read_line();                     //зчитуємо строку
        root = head;
        //неканонічний режим терміналу
        //зчитування символів в буфер
        //відловлення сигналів
    // while(head) {
    //     int i = 1;
    //     printf("\n*** Command = %s, \targs: ", head->cmd);
    //     while(head->av[i])
    //         printf("%s, ", head->av[i++]);
    //     printf(" ***\n");
    //     head = head->next;
    // }
        for ( ; root; root = root->next) {
            status = ush_execute(root);                 //виконуємо команди
        }
        

    } while(status);
}
/////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[], char * envp[]) {
    ush_loop();         //цикл команд

    return 0;
}

