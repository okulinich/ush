#include "libmx.h"

char *mx_strdel_ch(char **ptr) {
	if (ptr && *ptr) {
		free(*ptr);
		*ptr = NULL;
	}
	return NULL;
}
