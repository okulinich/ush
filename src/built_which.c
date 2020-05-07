#include "ush.h"

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
			else if(search_all_bins)
				rewrite_res(res, token_and_bin[0], token_and_bin[1]);
			else {
				closedir(search_dir);
				return "res";
			}
		}
	}
	return NULL;
}

static char *check_and_get_path(char *token, char *path_var, char *tmp[2],
						  char *bin_name, bool search_all_bins) {
	char *res = NULL;
	DIR *search_dir;

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

char *mx_get_path_to_binary(char *bin_name, bool search_all_bins) {
	char *temp_str = ssearch_for_var_in_env("PATH");
	char *path_var = mx_strdup(&(temp_str)[1]);
	char *token = strtok(path_var, ":");
	char *tmp[2];
	
	free(temp_str);
	return check_and_get_path(token, path_var, tmp, bin_name, search_all_bins);
}
