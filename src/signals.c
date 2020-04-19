#include <ush.h>

void mx_handler() {
	if (signal(SIGINT, SIG_IGN)) { // esli ctr+c to ignore
		mx_printstr("\n");
	}
	return;
}
