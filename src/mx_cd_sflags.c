#include "ush.h"

static void cd_one_arg(char *av, char **env) {
    struct stat buf;
    char *oldpwd = strdup(mx_get_env_var(env, "OLDPWD"));
    char *pwd = strdup(mx_get_env_var(env, "PWD"));
    char *ptr = NULL;

    lstat(av, &buf);
    if (chdir(av) == -1)
        fprintf(stderr, "cd: %s: %s\n", strerror(errno), av);
    ptr = getcwd(NULL, 0); // likuet func getcwd!!!
    setenv("OLDPWD", pwd, 1);
    // setenv("PWD", ptr, 1);
    mx_set_env_var("OLDPWD", oldpwd, &env);
    mx_set_env_var("PWD", ptr, &env);
    if (!(S_ISLNK(buf.st_mode)))
        mx_setenv_correct_path();
    mx_strdel(&oldpwd);
    mx_strdel(&pwd);
    mx_strdel(&ptr);
}

void mx_cd_sflags(char *av, char **env) {
    if (!av) { // go home !
        chdir(mx_get_env_var(env, "HOME"));
        setenv("OLDPWD", mx_get_env_var(env, "PWD"), 1);
        setenv("PWD", mx_get_env_var(env, "HOME"), 1);
        mx_set_env_var("PWD", mx_get_env_var(env, "HOME"), &env);
    }
    else {
        cd_one_arg(av, env);
        // system("leaks -q ush");
        // exit(1);
    }
}

void mx_setenv_correct_path() {
    char cur_path[PATH_MAX];

    getwd(cur_path);
    setenv("PWD", cur_path, 1);
    // free(cur_path);
}
