#include <ush.h>

void mx_handler() {
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
	return;
}
