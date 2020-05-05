#include <ush.h>

void mx_nosig(void) {
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
}

void mx_handler(void) {
	if (SIGINT)
		mx_printchar('\n');
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}
