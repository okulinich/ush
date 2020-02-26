#include "ush.h"

// static int get_flags(char **av) {
//     bool minus = 0;
//     int val = 0;

//     for (int i = 1; av[i] != NULL; i++) {
//         if (av[i][0] == '-' && !minus) {
//             if ((av[i][1] == '-' && av[i][2] == '\0') || av[i][1] == '\0')
//                 minus = 1; // если идет только - то false
//             for (int j = 1; av[i][j]; j++) { // zahodit -
//                 if (av[i][j] == 'P')
//                     val = 1; // esli -P to 1
//                 if (av[i][j] == 'L')
//                     val = 2; // esli -L to 2
//                 else if (av[i][j] != 'L' && (av[i][j] != '-'
//                         || (av[i][j] == '-' && j == 2))) { // esli ne -L i ne -- ili --
//                     fprintf(stderr, "pwd: bad option: -%c\n", av[i][j]);
//                     return -1;
//                 }
//             }
//         }
//         else {
//             fprintf(stderr, "pwd: too many arguments\n");
//             return -1 ;
//         }
//     }
//     return val;
// }


int mx_builtin_pwd(t_lst *list) { // сделать usage!!  переделать парсер фалагов !!!!!!!! (примеры pwd w, pwd -qwe, pwd 123, pwd 213)
    // int flag;

    // if ((flag = get_flags(list->av)) == 0)
    //     return puts(getcwd(NULL, 0));
    // else if ((flag = get_flags(list->av)) == -1)
    //     printf("FLAG -1");// return 0;
    // else if ((flag = get_flags(list->av)) == 2)
    //     printf("flag == 2\n");
    // else if ((flag = get_flags(list->av)) == 1)
    //     printf("flag == 1\n");
    if (list->av == NULL)
        puts(getcwd(NULL, 0));
    else
        puts(getwd(list->pwd));
    // if ((physical = get_flags(list->av)) != -1) {
    //     if (physical)
    //         puts(getwd(NULL));
    //     else
    //         puts(list->pwd);
    // }
    printf("\nbuilt pwd\n");
    return 1;
}
