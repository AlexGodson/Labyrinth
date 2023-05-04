#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
// Directory handling
#include <sys/types.h>
#include <dirent.h>

#include "../include/common.h"
#include "../include/filepath.h"
#include "../include/term.h"
#include "../include/directory.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("ERROR: incorrect arguments passed to main\n");
		printf("./main <filepath>\n");
		exit(1);
	}
	char *usr_path = argv[1]; // Path provided by the user in argv

	struct Filepath filepath;
	char *abs_path =path_clean(usr_path);
	
	if (!path_init(&filepath)) {
		fprintf(stderr, "%s:%d - ERROR: Initial Filepath not initialized\n",
				__FILE__, __LINE__);
	}
	path_split(&filepath, abs_path);

	path_assemble(&filepath, abs_path);
	printf("Assembled absolute path: %s\n", abs_path);

	struct termios term;
	if (!LR_setup_term(&term)) return_defer();

	// setup errors checked, program starting
	puts("welcome to the labyrinth\ngoodluck on your journey\n");
	
	printf("\e[1;1H\e[2J");
	puts("type 'q' to exit\n");
	printf("\e[?25l");

	int ret, dir_cursor = 0;
	struct dirent **dir_list =NULL;	

	int dir_sz = scandir(abs_path, &dir_list, NULL, alphasort);

	int counter = 0;
	while (counter < dir_sz) {
		if (counter == dir_cursor) printf("\e[4m\e[1m");
		printf("%d - %s\n", counter, dir_list[counter]->d_name);
		if (counter == dir_cursor) printf("\e[0m");
		counter++;
	}

	while (1) {
		char seq[32];
		memset(seq, 0, sizeof(seq));
		ret = read(STDIN_FILENO, seq, sizeof(seq));
		printf("\e[1;1H\e[2J");
		puts("type 'q' to exit\n");

		if (ret < 0) {
			fprintf(stderr, "error: something went wrong during reading user input: %s\n", strerror(errno));
			return_defer();
		}

		if (  !strcmp(seq, "q") || !(strcmp(seq, "q"))) break;
		if ( (!strcmp(seq, ES_U_ARROW) || !strcmp(seq, "k") ) && dir_cursor < dir_sz-1) dir_cursor--;
		if ( (!strcmp(seq, ES_D_ARROW) || !strcmp(seq, "j") ) && dir_cursor > 0) dir_cursor++;
		if (  !strcmp(seq, ES_NEWLINE) && dir_cursor)
/*
		if (  !strcmp(seq, ES_NEWLINE) && dir_cursor == dir_sz-1) {
			printf("uh oh\n");
			path_chop(&filepath);
			path_assemble(&filepath, abs_path);
			dir_sz = scandir(abs_path, &dir_list, NULL, alphasort);
			dir_cursor = 0;
		}
		if (  !strcmp(seq, ES_NEWLINE) && dir_cursor != 0) {
			printf("Big Boobies\n");
			path_append(&filepath, dir_list[dir_cursor]->d_name);
			path_assemble(&filepath, abs_path);
			dir_sz = scandir(abs_path, &dir_list, NULL, alphasort);
			dir_cursor = 0;
		}
*/	
		counter = 0;
		while (counter < dir_sz) {
			if (counter == dir_cursor) printf("\e[4m\e[1m");
			printf("%d - %s\n", counter, dir_list[counter]->d_name);
			if (counter == dir_cursor) printf("\e[0m");
			counter++;
		}
	}

//	for (int i = 0; i < 20; ++i) {
//		if (dir_list[i]) free(dir_list[i]);
//	}
	printf("\e[?25h");
	while (dir_sz--) {
		free(dir_list[dir_sz]);
	}
	free(dir_list);

	// Garbage collection
	LR_cleanup_term(&term);
	free(abs_path);
	path_free(&filepath.items);

	return 0;		
defer:
	fprintf(stderr, "%s:%d - ERROR: main program exiting\n",
			__FILE__, __LINE__);
	if (abs_path) free(abs_path);
	if (!(filepath.items)) path_free(&filepath.items);
	printf("\e[?25h");
	LR_cleanup_term(&term);
	return 1;
}

/* TODO:
	handle directories without malloc
	Stylized file browser in term
	Open on first runnin the program, not after input
	Jump into files to browse deeper
	Move up files (..)
	Browse files by search
	Capable of file deletion and creation
	Handle arguments better
	Remove Print_ES function from common	
*/
	/*
	while (1) {
		// scans directory at arg 1, soring them in a list of dir_pointers at &dir_list,
		// filtering by null, sorting alphabetically.
		// the size of the directory scanned is stored at dir_sz
		dir_sz = scandir(abs_path, &dir_list, NULL, alphasort);
		if (dir_sz == -1) return_defer();

		if (!strcmp(seq, "q") || !(strcmp(seq, "q"))) break;
		if ( (!strcmp(seq, ES_U_ARROW) || !strcmp(seq, "k") ) && dir_cursor < dir_sz-1) dir_cursor++;
		if ( (!strcmp(seq, ES_D_ARROW) || !strcmp(seq, "j") ) && dir_cursor > 0) dir_cursor--;
		if (!strcmp(seq, ES_NEWLINE)) {
			dir_cursor
		}

		while (dir_sz) {
			if (dir_sz == dir_cursor) printf("\e[4m\e[1m");
			printf("%s\n", dir_list[dir_sz]->d_name);
			if (dir_sz == dir_cursor) printf("\e[0m");
			free(dir_list[dir_sz--]);
		}
		free(dir_list);

		memset(seq, 0, sizeof(seq));
		ret = read(STDIN_FILENO, seq, sizeof(seq));
		printf("\e[1;1H\e[2J");
		puts("type 'q' to exit\n");

		if (ret < 0) {
			fprintf(stderr, "error: something went wrong during reading user input: %s\n", strerror(errno));
			return_defer();
		}
	}

	printf("\e[1;1H\e[2J");
*/
