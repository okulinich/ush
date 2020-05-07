#include "ush.h"


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

static char **realloc_env(int new_size, char **env) {
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
