#include "ush.h"

static void copy_environ(char **copy, char **environ) {
    for (int i = 0; environ[i]; i++) {
        copy[i] = strdup(environ[i]);
        if (environ[i + 1] == NULL)
            copy[i + 1] = NULL;
    }
}

char **mx_env_copy(void) {
    extern char **environ;
    char **env_copy = NULL;
    int len = 0;

    while (environ[len])
        len++;
    if (environ[0]) {
        env_copy = malloc((len + 100) * sizeof(char*));
        copy_environ(env_copy, environ);
    }
    return env_copy;
}

void mx_print_env(char **env) {
    for(int i = 0; env[i]; i++) {
        mx_printstr(env[i]);
        mx_printstr("\n");
    }
}

void mx_usage_env(char *flag) {
    mx_printerr("env: option requires an argument -- ");
    mx_printerr(flag);
    mx_printerr("\nusage: env [-i] [-P utilpath] [-u name]\n");
    mx_printerr("[name=value ...] [utility [argument ...]]\n");
}

char *mx_get_env_var(char **env, char *var) {   // возвращает ключ переменной который ты ищешь в массиве ЕНВ
	int	var_len;

	var_len = strlen(var);
	while (env && *env)
		if (!strncmp(var, *env, var_len) && (*env + var_len))
			return (*env + var_len + 1);
		else
			++env;
	return NULL;
}

int mx_find_env_var(char *key, char **env) {
    int i = -1;
    char *tmp = mx_strjoin(key, "=");

    while (env[++i]) {
        if (mx_get_substr_index(env[i], tmp) == 0) {
            free(tmp);
            return i;
        }
    }
    free(tmp);
    return i;
}

char **realloc_env(int new_size, char **env) {
    int i = -1;
	char **new;
    new = (char **)malloc(sizeof(char *) * (new_size + 1));;

	while (env[++i] && i < new_size) {
		new[i] = strdup(env[i]);
		free(env[i]);
	}
	return new;
}

void mx_set_env_var(char *key, char *value, char ***env) {
	int	pos = mx_find_env_var(key, *env);
	char *tmp = mx_strjoin("=", value);

	if (*(*env + pos)) {
		free(*(*env + pos));
		if (value)
			*(*env + pos) = mx_strjoin(key, tmp);
		else
			*(*env + pos) = mx_strjoin(key, "=");
	} else {
		*(env) = realloc_env(pos + 1, *env);
		if (value)
			*(*env + pos) = mx_strjoin(key, tmp);
		else
			*(*env + pos) = mx_strjoin(key, "=");
	}
    free(tmp);
}
