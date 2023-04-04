#ifndef _COMMON_H_
#define _COMMON_H_

#define return_defer() do {line = __LINE__; goto defer;} while(0);

#define ES_ESCAPE "\x1b"
#define ES_BACKSPACE "\x7f"
#define ES_NEWLINE "\x0a"
#define ES_UP "\x1b\x5b\x41"
#define ES_DOWN "\x1b\x5b\x42"
#define ES_RIGHT "\x1b\x5b\x43"
#define ES_LEFT "\x1b\x5b\x44"

#endif //_COMMON_H_