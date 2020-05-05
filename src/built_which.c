#include "ush.h"

static int parse_flags(bool *s_flag, bool *a_flag, t_lst *head) {
	int i = 1;

	if(mx_strcmp(head->av[1], "-") == 0)
		return 0;
	else if(head->av[1][0] != '-')
		return 1;
	else {
		for(; head->av[i] && head->av[i][0] == '-'; i++) {
			if (mx_strcmp(head->av[i], "-as") == 0 
				|| mx_strcmp(head->av[i], "-sa") == 0) {
				*s_flag = true;
				*a_flag = true;
			}
			else if(mx_strcmp(head->av[i], "-a") == 0)
				*a_flag = true;
			else if(mx_strcmp(head->av[i], "-s") == 0)
				*s_flag = true;
		}
	}
	return i;
}

static void rewrite_res(char **res, char *token, char *bin_name) {
	char *tmp;

	tmp = *res;
	*res = mx_strjoin(tmp, " ");
	free(tmp);
	tmp = *res;
	*res = mx_strjoin(tmp, token);
	free(tmp);
	tmp = *res;
	*res = mx_strjoin(tmp, "/");
	free(tmp);
	tmp = *res;
	*res = mx_strjoin(tmp, bin_name);
	free(tmp);
}

static char *check_dir(DIR *search_dir, char **res, char **token_and_bin, bool search_all_bins) {
	struct dirent *cur_dir;
	char *tmp;

	while((cur_dir = readdir(search_dir)) != NULL) {
		if(mx_strcmp(cur_dir->d_name, token_and_bin[1]) == 0) {
			if(*res == NULL) {
				tmp = mx_strjoin(token_and_bin[0], "/");
				*res = mx_strjoin(tmp, token_and_bin[1]);
				free(tmp);
			}
			else if(search_all_bins) {
				rewrite_res(res, token_and_bin[0], token_and_bin[1]);
			}
			else {
				closedir(search_dir);
				return "res";
			}
		}
	}
	return NULL;
}

static char *get_path_to_binary(char *bin_name, bool search_all_bins) {
	char *temp_str = ssearch_for_var_in_env("PATH");
	char *path_var = mx_strdup(&(temp_str)[1]);
	char *token = strtok(path_var, ":");
	char *res = NULL;
	char *tmp[2];
	DIR *search_dir;

	free(temp_str);
	while(token) {
		search_dir = opendir(token);
		if(search_dir == NULL) {
			token = strtok(NULL, ":");
			continue;
		}
		tmp[0] = token;
		tmp[1] = bin_name;
		if(check_dir(search_dir, &res, tmp, search_all_bins) != NULL) {
			free(path_var);
			return res;
		}
		token = strtok(NULL, ":");
		closedir(search_dir);
	}
	free(path_var);
	return res;
}

int mx_which(t_lst *head) {
	bool no_output = false;
	bool search_all_bins = false;
	int start_index = 0;
	char *res = NULL;
	int i;

	if(!head->av[1])
		return 1;
	start_index = parse_flags(&no_output, &search_all_bins, head);
	for(i = start_index; head->av[i]; i++) {
		res = get_path_to_binary(head->av[i], search_all_bins);
		if(check_if_cmd_is_builtin(head->av[i])) {
			mx_printstr(head->av[i]);
			mx_printstr(": shell built-in command\n");
		}
		if(no_output == false && res != NULL) {
			mx_printstr(res);
			mx_printstr("\n");
		}
		else if(no_output && res != NULL) {
			//hd->last_exit_status = 0;
			free(res);
			return 1;
		}
		else if(no_output && res == NULL) {
			return -1;
		}
		if(res)
			free(res);
	}
	return 1;
}

bool check_if_cmd_is_builtin(char *cmd) {
	if(mx_strcmp(cmd, "echo") == 0)
		return true;
	if(mx_strcmp(cmd, "env") == 0)
		return true;
	if(mx_strcmp(cmd, "cd") == 0)
		return true;
	if(mx_strcmp(cmd, "exit") == 0)
		return true;
	if(mx_strcmp(cmd, "export") == 0)
		return true;
	if(mx_strcmp(cmd, "unset") == 0)
		return true;
	if(mx_strcmp(cmd, "which") == 0)
		return true;
	if(mx_strcmp(cmd, "help") == 0)
		return true;
	if(mx_strcmp(cmd, "true") == 0)
		return true;
	if(mx_strcmp(cmd, "false") == 0)
		return true;
	if(mx_strcmp(cmd, "set") == 0)
		return true;
	if(mx_strcmp(cmd, "bye") == 0)
		return true;
	return false;
}
