#ifndef _FILEPATH_H_
#define _FILEPATH_H_

#include "./common.h"

#define MAX_PATH 10

struct Filepath {
	char **items;
	char *abs_path;
	int count;
	int capacity;
};

// Takes a filepath and turns it into a usable format
char *path_clean(char *path);

// Initialized the memory of struct Filepath *fp
ERRNO path_init(struct Filepath *fp);

// Splits path by "/" into array of pointers to strings.
// 'item' memory must be freed along with items itself.
void path_split(struct Filepath *fp, char *abs_path);

// Appends string 'file' to the next element of filepath.items
void path_append(struct Filepath *fp, const char *file);

// Chops the last file from the absolute path, decrementing count and freeing memory
void path_chop(struct Filepath *fp);

// Assembles the array of strings in struct Filepath to an absolute directory path
ERRNO path_assemble(struct Filepath *fp, char *dest);

// Frees the memory in '.items' and the pointer array
void path_free(char ***fp);

#endif //_FILEPATH_H_
