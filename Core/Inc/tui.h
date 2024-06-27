#ifndef H_TUI
#define H_TUI

struct menu_t {
	const wchar_t** items;
	int ref_x;
	int ref_y;
	int item_index;
	const int size_x;
	const int size_y;

	struct menu_t *prev_menu;
	int cur_x;
	int cur_y;
};

void moveCursor(int, int);
void set_noncanonical_mode(void);

int draw_box(int, int, int, int);

int draw_menu(struct menu_t*);
int clear_style(struct menu_t*);
int set_style(struct menu_t*);

#endif
