#ifndef TERM_H_
#define TERM_H_

// Terminal Handling
#include <termios.h>
#include <unistd.h>

int LR_setup_term(struct termios *term);
void LR_cleanup_term(struct termios *term);

#endif // TERM_H_
