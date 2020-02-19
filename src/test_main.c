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
#include <string.h>

#define BUFSIZE 1024
#define DELIMITERS "\t\r\n\a "
#include <string.h>

//builtin commands: export, unset, fg, exit
char *builtin_str[] = {"cd", "help", "exit"}; 

int ush_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int ush_cd(char **args) {
    if(args[1] == NULL) {
        fprintf(stderr, "ush: waiting argument for \"cd\"");
    }
    else {
        if(chdir(args[1]) != 0)     //встановлює в якості поочного каталог 
            perror("ush");          //на який вказує аргумент
    }
    return 1;
}

int ush_help(char **args) {
    int i;
    printf("-------------USH-------------\n");
    printf("- List of builtin commands: -");
    for(int i = 0; i < ush_num_builtins(); i++)
        printf("- %s *\n", builtin_str[i]);
    return 1;
}

int ush_exit(char **args) {
    return 0;
}

int (*builtin_func[]) (char **args) = { &ush_cd, &ush_help, &ush_exit };





////////////////////////////////////////звитування строки////////////////////////////
char *ush_read_line() {
    int buf_size = BUFSIZE;
    int pos = 0;
    char *buffer = (char *)malloc(sizeof(char) * buf_size);
    int c;

    while(1) {
        c = getchar();                      //зчитуємо символ

        if(c == EOF || c == '\n') {
            buffer[pos] = '\0';
            return buffer;
        }
        else {
            buffer[pos] = c;                //записуємо в строку (буфер)
        }
        pos++;

        if(pos >= buf_size) {               //перевіряємо розмір буфера
            buf_size += BUFSIZE;
            buffer = realloc(buffer, buf_size);
        }
    }
}

char *lsh_read_line(void)
{
  char *line = NULL;
  size_t bufsize = 0; // getline сама выделит память
  getline(&line, &bufsize, stdin);
  return line;
}
/////////////////////////////////////////////////////////////////////////////////////////




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
int ush_launch(char **args) {
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
        if(execvp(args[0], args) == -1)
            perror("ush");
        exit(1);
    }
    else if(pid < 0) {
        perror("ush");
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


int ush_execute(char **args) {
    int i;

    if(args[0] == NULL) {
        return 1;
    }

    //цикл перевіряє чи є команда вбудованою
    //якшо так, тоді викликає її
    //якшо ні, то запускає процес через ush_launch
    for(i = 0; i < ush_num_builtins(); i++) {
        if(strcmp(args[0], builtin_str[i]) == 0)
            return (*builtin_func[i])(args);
    }
    return ush_launch(args);
}



//////////////////////////////////////основний цикл/////////////////////////////////////
void ush_loop() {
    char *line;
    char **args;
    int status;

    do {
        printf("u$h> ");
        line = ush_read_line();                     //зчитуємо строку
        //неканонічний режим терміналу
        //зчитування символів в буфер
        //відловлення сигналів
        //
        args = ush_split_line(line);                //парсимо строку (виділяємо команди флаги і аргументи)
        status = ush_execute(args);                 //виконуємо команди

        free(line);
        free(args);
    } while(status);
}
/////////////////////////////////////////////////////////////////////////////////////////


int main() {
    
    ush_loop();         //цикл команд

    return 0;
}

