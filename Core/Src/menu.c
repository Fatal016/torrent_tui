#include <wchar.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#include "../Inc/menu.h"

int draw_menu(struct menu_t *menu) {
	draw_box(menu->size_x, menu->size_y, menu->ref_x, menu->ref_y);
	for (int i = 0; i < menu->size_y; i++) {
		moveCursor(menu->ref_x + 2, menu->ref_y + 1 + i);
		wprintf(L"%ls", ((struct menu_t**)menu->items)[i]->pretty_name);
	}
	return 0;
}

int draw_field(struct menu_t *menu) {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	int size_x = w.ws_col - 2;
	int size_y = w.ws_row - 2;

	if (w.ws_col > menu->size_x) {
		size_x = menu->size_x;
	}

	if (w.ws_row > menu->size_y) {
		size_y = menu->size_y;
	}
	draw_box(size_x, size_y, menu->ref_x, menu->ref_y);
	
	for (int i = 0; i < size_y; i++) {
		moveCursor(menu->ref_x + 2, menu->ref_y + 1 + i);
		wprintf(L"%ls %ls", ((struct field_t**)menu->items)[i]->field_name, ((struct field_t**)menu->items)[i]->field_value);
	}
	return 0;
}

int clear_style(struct menu_t *menu) {
	
	/* Move cursor to beginning of line */
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);

	/* Reset text highlighting */
	if (menu->type == MENU) {
		wprintf(L"\033[0m%*s", menu->size_x + 1, "");
	} else if (menu->type == FIELD) {
		wprintf(L"\033[0m%*s", wcslen(((struct field_t**)menu->items)[menu->cur_y - 1]->field_name) + 1, "");
	}

	/* Move cursor back to beginning of line */
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);

	/* Redraw text with default text color */
	if (menu->type == MENU) {
		wprintf(L"\033[0m %ls", ((struct menu_t**)menu->items)[menu->cur_y - 1]->pretty_name);
	} else if (menu->type == FIELD) {
		wprintf(L"\033[0m %ls", ((struct field_t**)menu->items)[menu->cur_y - 1]->field_name);
	}

	return 0;
}

int set_style(struct menu_t *menu) {
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);
	
	if (menu->type == MENU) {
		wprintf(L"\033[47m%*s", menu->size_x + 1, "");
	} else if (menu->type == FIELD) {
		wprintf(L"\033[47m%*s", wcslen(((struct field_t**)menu->items)[menu->cur_y - 1]->field_name) + 1, "");
	}

	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);

if (menu->type == MENU) {
		wprintf(L"\033[30m %ls", ((struct menu_t**)menu->items)[menu->cur_y - 1]->pretty_name);
	} else if (menu->type == FIELD) {
		wprintf(L"\033[30m %ls", ((struct field_t**)menu->items)[menu->cur_y - 1]->field_name);
	}
	return 0;
}



int draw_box(int size_x, int size_y, int ref_x, int ref_y) 
{
	wprintf(L"\033[%d;%dH", ref_y, ref_x);

	/* Upper bar */
	wprintf(L"%lc", 0x250C);
	for (int i = 0; i < size_x + 1; i++) {
		wprintf(L"%lc", 0x2500);
	}
	wprintf(L"%lc\n", 0x2510);

	/* Sides */
	for (int i = 0; i < size_y; i++) {
		if (ref_x != 1) {
			wprintf(L"\033[%dC", ref_x-1);
		}
		wprintf(L"%lc\033[%dC%lc\n", 0x2502, size_x + 1, 0x2502);
	}	

	/* Lower Bar */
	if (ref_x != 1) {
		wprintf(L"\033[%dC", ref_x-1);
	}
	wprintf(L"%lc", 0x2514);
	

	for (int i = 0; i < size_x + 1; i++) {
		wprintf(L"%lc", 0x2500);
	}
	wprintf(L"%lc", 0x2518);

	return 0;
}


int resize_menu(struct menu_t* menu) {
	menu->size_x = max_size(menu) + 1;
	return 0;
}


int max_size(struct menu_t* menu)
{
	size_t max = 0, len;

	for (int i = 0; i < menu->size_y; i++) {
		len = 0;

		if (menu->type == MENU) {
			len = wcslen(((struct menu_t**)menu->items)[i]->pretty_name);
		} else {
			len = wcslen(((struct field_t**)menu->items)[i]->field_name) + 1;
			if (((struct field_t**)menu->items)[i]->field_value != NULL) {
				len += wcslen(((struct field_t**)menu->items)[i]->field_value);
			} else {
				len += 6;
			}
		}
		if (len > max) {
			max = len;
		}
	}

	return (int)max;
}
