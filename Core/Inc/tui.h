#ifndef H_TUI
#define H_TUI

#define UP_ARROW 	65
#define DOWN_ARROW	66 
#define RIGHT_ARROW	67
#define LEFT_ARROW	68



struct menu_t {

	/*************************/
	/* Complile Time Members */
	/*************************/
	
	struct menu_t *prev_menu;
	struct menu_t *next_menu;


	/*******************/
	/* Runtime Members */
	/*******************/	

	/* Textual fields in the menu */
	/* For high-level menus, this member is compile-time */
	wchar_t** items;
	
	/* Reference coordinate for menu. Top left corder of terminal, 1 indexed */
	int ref_x;
	int ref_y;
	
	/* X and Y size of menu */
	int size_x;
	int size_y;
	
	/* Current X and Y coordinates of menu */
	int cur_x;
	int cur_y;

	/* Index of item currently hovered over */
	int item_index;

};

void moveCursor(int, int);
void set_noncanonical_mode(void);
wchar_t* strtowstr(char*);

#endif
