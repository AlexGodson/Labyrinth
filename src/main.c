#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "../include/common.h"

// Terminal Handling
#include <termios.h>
#include <unistd.h>

// Directory handling
#include <sys/types.h>
#include <dirent.h>

int LR_setup_term(struct termios *term);
void LR_cleanup_term(struct termios *term);

int main(int argc, char **argv) {
    int line = 0;
    if (argc != 2) {
        printf("ERROR: Incorrect arguments passed to main\n");
        printf("./main <FILEPATH>\n");
        exit(1);
    }

    struct termios term;

    if (!LR_setup_term(&term)) return_defer();

    // Setup Errors Checked, Program starting
    puts("Welcome to the Labyrinth \nGoodluck on Your Journey\n");

    while (1) {
        char seq[32];
        memset(seq, 0, sizeof(seq));
        int ret = read(STDIN_FILENO, seq, sizeof(seq));
        if (!strcmp(seq, ES_ESCAPE)) break;
        char *bruh = "UP_ARROW";
        if (!strcmp(seq, ES_UP)) { printf("\e[4m%s\e[0m\n", bruh);}
        if (ret < 0) {
            fprintf(stderr, "ERROR: something went wrong during reading user input: %s\n", strerror(errno));
            return_defer();
        }
        assert(ret < 32);
        printf("\"");
        for (int i = 0; i < ret; ++i) {
            printf("\\x%02x", seq[i]);
        }
        printf("\"\n");
    }

    DIR *derp = opendir(*(argv+1));

    struct dirent *D = readdir(derp);

    while (D) {
        printf("%s\n", D->d_name);
        D = readdir(derp);
    }

    LR_cleanup_term(&term);

    return 0;

defer:
    printf("ERROR: __LINE__(%d)\n", line);
    LR_cleanup_term(&term);
    return 1;
}

int LR_setup_term(struct termios *term) {
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        fprintf(stderr, "ERROR: Please run Labyrinth in a Terminal\n");
    }

    if (tcgetattr(STDIN_FILENO, term)) {
        fprintf(stderr, "ERROR: Could not get terminal attributes\n(%d): %s", __LINE__, strerror(errno));
        return 0;
    }

    term->c_lflag &= ~ECHO;
    term->c_lflag &= ~ICANON;

    if (tcsetattr(0, 0, term)) {
        fprintf(stderr, "ERROR: could not update the state of the terminal: (%d): %s\n", __LINE__, strerror(errno));
        return 0;
    }

    return 1;
}

void LR_cleanup_term(struct termios *term) {
    term->c_lflag |= ECHO;
    term->c_lflag |= ICANON;
    if (tcsetattr(STDIN_FILENO, 0, term)) {
        fprintf(stderr, "FATAL ERROR: COULD NOT RESTORE TERMINAL\nTHE WALLS ARE CAVING IN\n(%d): %s\n", __LINE__, strerror(errno));
        exit(1);
    }
}
