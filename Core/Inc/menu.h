#include "tui.h"

#ifndef H_MENU
#define H_MENU


typedef enum {
	MENU,
	FIELD
} menu_type;




struct field_t {
	
	wchar_t *field_name;
	wchar_t *field_value;
};

struct menu_t {

	/*************************/
	/* Complile Time Members */
	/*************************/
	
	struct menu_t *prev_menu;
	
	/* Display name for menu in parent menu */
	wchar_t *pretty_name;

	/*******************/
	/* Runtime Members */
	/*******************/	

	/* Textual fields in the menu */
	/* For high-level menus, this member is compile-time */

	//	wchar_t** items;
	//struct menu_t** items;

	void **items;

	menu_type type;

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

int max_size(struct menu_t*);

int resize_menu(struct menu_t*);

int draw_box(int, int, int, int);
int draw_menu(struct menu_t*);
int draw_field(struct menu_t*);

int clear_style(struct menu_t*);
int set_style(struct menu_t*);

#endif
