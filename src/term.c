#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//
// Terminal Handling
#include <termios.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/term.h"

int LR_setup_term(struct termios *term) {
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)){
		fprintf(stderr, "ERROR: please run labyrinth in a terminal\n");
	}

	if (tcgetattr(STDIN_FILENO, term)) {
		fprintf(stderr, "%s:%d - ERROR: could not get terminal attributes\n:%s",
				__FILE__, __LINE__, strerror(errno));
		return ERROR;
	}

	term->c_lflag &= ~ECHO;
	term->c_lflag &= ~ICANON;

	if (tcsetattr(0, 0, term)) {
		fprintf(stderr, "%s:%d - ERROR: Could not update terminal attributes\n:%s",
				__FILE__, __LINE__, strerror(errno));
		return ERROR;
	}

	return SUCCESS;
}

void LR_cleanup_term(struct termios *term) {
	term->c_lflag |= ECHO;
	if (tcsetattr(STDIN_FILENO, 0, term)) {
		fprintf(stderr, "%s:%d - FATAL ERROR: could not restore terminal\nthe walls are caving in\n%s\n",
				__FILE__, __LINE__, strerror(errno));
		exit(1);
	}
}
