#ifndef H_TUI
#define H_TUI

#define UP_ARROW 	65
#define DOWN_ARROW	66 
#define RIGHT_ARROW	67
#define LEFT_ARROW	68

void moveCursor(int, int);
void set_noncanonical_mode(void);

void handle_signal(int);

#endif
