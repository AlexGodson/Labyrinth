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

int main(int argc, char **argv) {
    int line = 0;
    if (argc != 2) {
        printf("ERROR: Incorrect arguments passed to main\n");
        printf("./main <FILEPATH>\n");
        exit(1);
    }

    // Checking the program is being run within a valid terminal
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        fprintf(stderr, "ERROR: Please run the editor in the terminal!\n(%d): %s", errno, strerror(errno));
        exit(1);
    }

    struct termios terminal = {0};

    if (tcgetattr(STDIN_FILENO, &terminal)) {
        fprintf(stderr, "ERROR: Could not get terminal attributes\n(%d): %s", errno, strerror(errno));
        return_defer();
    }

    terminal.c_lflag &= ~ECHO;
    terminal.c_lflag &= ~ICANON;

    if (tcsetattr(0, 0, &terminal)) {
        fprintf(stderr, "ERROR: could not update the state of the terminal: (%d): %s\n", errno, strerror(errno));
        return_defer();
    }

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

    puts("Welcome to the Labyrinth \nGoodluck on Your Journey\n");

    DIR *derp = opendir(*(argv+1));

    struct dirent *D = readdir(derp);

    while (D) {
        printf("%s/%s\n", *(argv+1), D->d_name);
        D = readdir(derp);
    }

    terminal.c_lflag |= ECHO;
    terminal.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, 0, &terminal);

    return 0;

defer:
    printf("ERROR: __LINE__(%d)", line);
    printf("\033[2J");
    terminal.c_lflag |= ECHO;
    terminal.c_lflag |= ICANON;
    tcsetattr(STDIN_FILENO, 0, &terminal);
}
