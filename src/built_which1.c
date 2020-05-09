#include <ush.h>

bool mx_check_if_cmd_is_builtin(char *cmd) {
	char *com[] = {"echo", "env", "set", "cd", "exit", "export", "unset", 
				   "which", "help", "true", "false", "bye", NULL};

	for (int i = 0; com[i]; i++)
		if (mx_strcmp(cmd, com[i]) == 0)
			return true;
	return false;
}


static int ret_cycle(int start_index, t_lst *head, char *res, 
				  bool search_all_bins, bool no_output) {
	for (int i = start_index; head->av[i]; i++) {
		res = mx_get_path_to_binary(head->av[i], search_all_bins);
		if (mx_check_if_cmd_is_builtin(head->av[i])) {
			mx_printstr(head->av[i]);
			mx_printstr(": shell built-in command\n");
		}
		if (no_output == false && res != NULL) {
			mx_printstr(res);
			mx_printstr("\n");
		}
		else if (no_output && res != NULL) {
			free(res);
			return 1;
		}
		else if (no_output && res == NULL)
			return -1;
		if (res)
			free(res);
	}
	return 1;
}

static int parse_flags(bool *s_flag, bool *a_flag, t_lst *head) {
	int i = 1;

	if (mx_strcmp(head->av[1], "-") == 0)
		return 0;
	else if (head->av[1][0] != '-')
		return 1;
	else {
		for (; head->av[i] && head->av[i][0] == '-'; i++) {
			if (mx_strcmp(head->av[i], "-as") == 0 
				|| mx_strcmp(head->av[i], "-sa") == 0) {
				*s_flag = true;
				*a_flag = true;
			}
			else if (mx_strcmp(head->av[i], "-a") == 0)
				*a_flag = true;
			else if (mx_strcmp(head->av[i], "-s") == 0)
				*s_flag = true;
		}
	}
	return i;
}

int mx_which(t_lst *head) {
	bool no_output = false;
	bool search_all_bins = false;
	int start_index = 0;
	char *res = NULL;

	if (!head->av[1])
		return 1;
	start_index = parse_flags(&no_output, &search_all_bins, head);
	return ret_cycle(start_index, head, res, search_all_bins, no_output);
}
