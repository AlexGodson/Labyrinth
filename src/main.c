#include <stdio.h>
#include <stdlib.h>

// Terminal Handling
#include <termios.h>
#include <unistd.h>

// Directory handling
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("ERROR: Incorrect arguments passed to main\n");
        printf("./main <FILEPATH>\n");
        exit(1);
    }

    struct termios term_reset;

    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        fprintf(stderr, "ERROR: Please run the editor in the terminal!\n");
        exit(1);
    }

    int err = tcgetattr(fd, &term_reset);

    puts("Welcome to the Labyrinth \nGoodluck on Your Journey\n");

    DIR *derp = opendir(*(argv+1));

    struct dirent *D = readdir(derp);

    while (D) {
        printf("%s/%s\n", *(argv+1), D->d_name);
        D = readdir(derp);
    }

    return 0;
}