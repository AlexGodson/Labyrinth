#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "../include/filepath.h"
#include "../include/common.h"

#define MAX_NAME_SZ 257

#define DIR_NAME_LEN 1024

// Trims unnecessary characters from the path
void path_trim(char *abs_path) {
	int str_len = strlen(abs_path) + 1;
	for (int i = 1; i < str_len; ++i) {
		if (abs_path[i] == '/' && abs_path[i] == abs_path[i-1]) {
			memmove(&abs_path[i-1], &abs_path[i], str_len - i);
			--i;
		}
		if (abs_path[str_len-2] == '/') {
			abs_path[str_len-2] = '\0';
		}
	}
}

// Takes a filepath and turns it into a usable format
char *path_clean(char *path) {
	char *abs_path = malloc(sizeof(*abs_path) * DIR_NAME_LEN); 
	if (strcmp(path, ".") == 0) {
		if (!getcwd(abs_path, DIR_NAME_LEN)) {
			fprintf(stderr, "%s:%d - ERROR: Could not get current working directory\n%s",
					__FILE__, __LINE__, strerror(errno));
			strcpy(abs_path, "/home/alexg");
		}
	} else {
		strcpy(abs_path, path);
		path_trim(abs_path);
	}

	return abs_path;
}

// Initialized the memory of struct Filepath *fp
ERRNO path_init(struct Filepath *fp) {
	fp->items = calloc(sizeof(*fp->items), MAX_PATH);
	fp->capacity = MAX_PATH;
	fp->count = 0;
	if (!fp->items) {
		fprintf(stderr, "%s:%d - ERROR: Could not initialize struct Filepath\n",
				__FILE__, __LINE__);
		free(fp->items);
		return ERROR;
	} 
	return SUCCESS;
}

// Appends string 'file' to the next element of filepath.items
void path_append(struct Filepath *fp, const char *file) {
	ASSERT(fp != NULL, "struct Filepath invalid, cannot append %s", file);
	if (fp->count >= fp->capacity) {
		fp->capacity = fp->capacity == 0 ? MAX_PATH : fp->capacity*2;
		fp->items = realloc(fp->items, fp->capacity * sizeof(*(fp)->items));
		ASSERT(fp->items != NULL, "No more Ram :(\n");
	}
	fp->items[fp->count] = calloc(sizeof(*fp->items), MAX_NAME_SZ);
	strcpy(fp->items[fp->count++], file);
}

// Chops the last file from the absolute path, decrementing count and freeing memory
void path_chop(struct Filepath *fp) {
	if (fp->count == 0) return;
	free(fp->items[fp->count--]);
}

// Splits path by "/" into array of pointers to strings.
// 'item' memory must be freed along with items itself.
void path_split(struct Filepath *fp, char *abs_path) {
	char buf[MAX_NAME_SZ];
	while(1) {
		char *temp = strsep(&abs_path, "/");
		if (temp == NULL) break;
		size_t temp_len = strlen(temp);

		memset(buf, '\0', MAX_NAME_SZ);
		strncpy(buf, temp, temp_len + 1);
		buf[temp_len] = '/';
		path_append(fp, buf);
	}
}

// Assembles the array of strings in struct Filepath to an absolute directory path
ERRNO path_assemble(struct Filepath *fp, char *dest) {
	memset(dest, '\0', DIR_NAME_LEN);
	for (int i = 0; i < fp->count; ++i) {
		strcat(dest, fp->items[i]);
	}
	return SUCCESS;
}

// Frees the memory in '.items' and the pointer array
void path_free(char  ***fp) {
	if (*fp == NULL && (*fp)[0] == NULL) return;
	int i = 0;
	while (*fp && (*fp)[i] != NULL) {
		free((*fp)[i++]);
	}
	free(*fp);
}
