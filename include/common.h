#ifndef _COMMON_H_
#define _COMMON_H_

#define ES_ESCAPE		"\x1b"
#define ES_BACKSPACE		"\x7f"
#define ES_NEWLINE		"\x0a"
#define ES_U_ARROW		"\x1b\x5b\x41"
#define ES_D_ARROW		"\x1b\x5b\x42"
#define ES_DOWN			"\x1b\x5b\x42"
#define ES_RIGHT		"\x1b\x5b\x43"
#define ES_LEFT			"\x1b\x5b\x44"
//#include <unistd.h>
#include <assert.h>

#define ERROR 0
#define SUCCESS 1
#define ERRNO int

#define return_defer() do {goto defer;} while(0);

#define ASSERT(cond, ...) do { 								\
	if (!(cond)) { 									\
		fprintf(stderr, "%s:%d: ASSERTION FAILED: ", __FILE__, __LINE__); 	\
		fprintf(stderr, __VA_ARGS__); 						\
		fprintf(stderr, "\n"); 							\
		exit(1); 								\
	} 										\
} while (0)

#define da_append(da, item) do {                                                        \
	if ((da)->count >= (da)->capacity) {                                            \
		(da)->capacity = (da)->capacity == 0 ? MAX_PATH : (da)->capacity*2;     \
		(da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items));\
		ASSERT((da)->items != NULL, "Buy more RAM lol");                        \
	}                                                                               \
	(da)->items[(da)->count++] = (item);                                            \
} while (0)	

// REMOVE ME WHEN THE PROJECT IS COMPLETE
// void LR_print_es(char *seq, int sz) {
// 	if (!seq) {
// 		printf("error: escape sequence is null\n");
// 		return;
// 	}
// 	for (int i = 0; i < sz; ++i) {
// 		printf("\\x%02x", seq[i]);
// 	}
// 	printf("\"\n");
// }


#endif //_COMMON_H_
