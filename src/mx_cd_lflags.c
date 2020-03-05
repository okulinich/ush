#include "ush.h"


static void cd_with_min_flag(char **env) {
    char *pwd = strdup(mx_get_env_var(env, "PWD"));
    char *oldpwd = strdup(mx_get_env_var(env, "OLDPWD"));

    chdir(oldpwd);
    setenv("PWD", oldpwd, 1);
    setenv("OLDPWD", pwd, 1);
    mx_set_env_var("OLDPWD", pwd, &env);
    mx_set_env_var("PWD", oldpwd, &env);
    mx_setenv_correct_path();
    mx_strdel(&pwd);
    mx_strdel(&oldpwd);
}

void mx_cd_lflags(char *av, char **env, char flag) {
        if (flag == 4) {
            cd_with_min_flag(env);
        }
        if (av)
            mx_printstr("AV)))\n");
    //     else if (flags_cd->s_flag) {
    //         check_lnk_s_flag(splited_arg, dirs_cd, errors);
    //     }
    //     else {
    //         check_lnk_p_flag(splited_arg, dirs_cd);
    //     }
    // }
    // system("leaks -q")
}
