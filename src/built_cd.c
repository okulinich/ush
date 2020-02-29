#include "ush.h"

char **add_string_to_arr(char **arr, char *path) {
	int	len;
	char **tmp = arr;

	if ((len = 2) && arr) {
		while (*arr && ++len)
			++arr;
		if ((arr = (char **)malloc(len * sizeof(char *)))) {
			len = -1;
			while (tmp[++len])
				arr[len] = strdup(tmp[len]);
			arr[len++] = strdup(path);
			arr[len] = NULL;
		}
		return arr;
	}
	else if ((arr = (char **)malloc(len * sizeof(char *))))	{
		*arr = strdup(path);
		*(arr + 1) = NULL;
		return arr;
	}
	return NULL;
}

int	 mx_print_error(char *file, char *err) {
	printf("ush: %s %s\n", file, err);
	return 0;
}

static int	set_old_pwd(t_global *s, char *old_pwd) {
	char **env;
	char *tmp;

	env = s->env;
	while (*env) {
		if (!mx_strncmp("OLDPWD=", *env, 7) && (tmp = *env)) {
			*env = mx_strjoin("OLDPWD=", old_pwd);
			mx_strdel(&old_pwd);
			mx_strdel(&tmp);
			return 0;
		}
		++env;
	}
	env = s->env;
	tmp = mx_strjoin("OLDPWD=", old_pwd);
	s->env = add_string_to_arr(env, tmp);
	mx_strdel(&tmp);
	mx_strdel(&old_pwd);
    mx_del_strarr(&env);
	return 0;
}

static	int	set_pwd(t_global *s) {
	char **env;
	char *tmp;
	char *new_pwd;

	env = s->env;
	if (!(tmp = getcwd(NULL, 0)))
		return (mx_print_error("ERROR in set_pwd()", "getcwd()"));
	while (*env) {
		if (!strncmp("PWD=", *env, 4) && !mx_strdel_ch(&(*env))) {
			*env = mx_strjoin("PWD=", tmp);
			mx_strdel(&tmp);
			return (0);
		}
		++env;
	}
	env = s->env;
	new_pwd = mx_strjoin("PWD=", tmp);
	s->env = add_string_to_arr(env, new_pwd);
	mx_strdel(&new_pwd);
	mx_strdel(&tmp);
	mx_del_strarr(&env);
	return 0;
}

static int	check_cd_errors(char *pathname) {
	if (!pathname)
		return 1;
	if (access(pathname, F_OK) == -1 &&
			!mx_print_error(pathname, "no such file or directory"))
		return 1;
	if (access(pathname, X_OK) == -1 &&
			!mx_print_error(pathname, "permission denied"))
		return 1;
	return 0;
}

static char	**get_path(char **argv, char **env) {
	char	*argument;
	char	*env_value;

	argument = *(argv + 1);
	if (argument && strcmp(argument, "-"))
		return (argv + 1);
	else if (argument) {
		if ((env_value = mx_get_env_var(env, "OLDPWD"))) {
			mx_strdel(&(*argv));
			*argv = strdup(env_value);
			return argv;
		}
		else
			return (argv + 1);
	}
	else if ((env_value = mx_get_env_var(env, "HOME"))) {
		mx_strdel(&(*argv));
		*argv = strdup(env_value);
		return argv;
	}
	return (argv + 1);
}

int	mx_cd(t_global *s, t_lst *h) {
	char **argv = h->av;
	char *old_pwd;

	argv = get_path(argv, s->env);
	if (check_cd_errors(*argv))
		return 0;
	if (!(old_pwd = getcwd(NULL, 0)) && !mx_print_error(*argv, "ERROR getcwd()"))
		return (0);
	if (chdir(*argv) == -1 && !mx_print_error(*argv, "ERROR chdir()") &&
		!mx_strdel_ch(&old_pwd))
		return (0);
	set_old_pwd(s, old_pwd);
	set_pwd(s);
	return 1;
}
