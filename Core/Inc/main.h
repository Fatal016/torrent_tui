struct menu_t {
	wchar_t** items;
	int ref_x;
	int ref_y;
	int item_index;
	int size_x;
	int size_y;

	int cur_x;
	int cur_y;
};

int max_size(wchar_t**, int);

void moveCursor(int, int);
void set_noncanonical_mode(void);

int draw_box(int, int, int, int);

int draw_menu(struct menu_t*);
int clear_style(struct menu_t*);
int set_style(struct menu_t*);
