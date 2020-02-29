#include "ush.h"

// bool is_flag_stop(char *flag) {
//     if ((flag[1] == '-' && flag[2] == '\0')
//         || flag[1] == '\0') {
//         return true;
//     }
//     return false;
// }

// static bool parse_flags(char **flags, bool *mode) {
//     bool flag_stop = 0;

//     for (int i = 1; flags[i]; i++) {
//         if (flags[i][0] == '-' && !flag_stop) {
//             flag_stop = is_flag_stop(flags[i]);
//             for (int j = 2; flags[i][j] != '\0'; j++)
//                 if (flags[i][j] == 'P')
//                     *mode = 1;
//                 else if (flags[i][j] != 'L' && (flags[i][j] != '-'
//                          || (flags[i][j] == '-' && j == 2))) {
//                     fprintf(stderr, "pwd: bad option: -%c\n", flags[i][j]);
//                     return 1;
//                 }
//         }
//         else {
//             fprintf(stderr, "pwd: too many arguments\n");
//             return 1;
//         }
//     }
//     return 0;
// }

int mx_builtin_pwd(t_global *hd) { // сделать usage!!  переделать парсер фалагов !!!!!!!! (примеры pwd w, pwd -qwe, pwd 123, pwd 213)
    // bool flag = 0;
    char *pwd = mx_get_env_var(hd->env, "PWD");
    char *tmp = NULL;

    // if (!parse_flags(hd->new->av, &flag)) {
    if (hd->new->av[1] != NULL) {
        if (strcmp(hd->new->av[1], "P")) {
        tmp = realpath(pwd, NULL);
        printf("IF:%s\n", tmp);
        mx_strdel(&tmp);
        }
    }
    else
        printf("ELSE:%s\n", pwd);
    printf("\nbuiltin\n");
    return 1;
}
