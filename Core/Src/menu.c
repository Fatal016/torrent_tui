#include <wchar.h>

#include "../Inc/menu.h"
/*
wchar_t *torrent_info_menu_items[] = {
	L"Filename  : ",
	L"Info Hash : "
};
*/


struct field_t torrent_info_menu_items[] = {
	{L"Filename : ", NULL},
	{L"Info Hash: ", NULL}
};



struct menu_t torrent_info_menu = {
	.pretty_name = L"Torrent Info",
	.items = (void *)torrent_info_menu_items,
	.type = FIELD,	
	.ref_x = 1,
	.ref_y = 1,
	.item_index = 0,
	.size_y = 2,
	.prev_menu = &category_menu,
	.cur_x = 0,
	.cur_y = 0
};

struct menu_t tracker_info_menu = {
	.pretty_name = L"Tracker Info",
	.ref_x = 1,
	.ref_y = 1,
	.item_index = 0,
	.size_y = 100,
	.prev_menu = &category_menu,
	.cur_x = 0,
	.cur_y = 0
};



int draw_menu(struct menu_t *menu) {
	draw_box(menu->size_x, menu->size_y, menu->ref_x, menu->ref_y);
	for (int i = 0; i < menu->size_y; i++) {
		moveCursor(menu->ref_x + 2, menu->ref_y + 1 + i);
		wprintf(L"%ls", ((struct menu_t**)menu->items)[i]->pretty_name);
	}
	return 0;
}

int draw_field(struct menu_t *menu) {
	
	menu->size_x = 30;
	draw_box(menu->size_x, menu->size_y, menu->ref_x, menu->ref_y);
	for (int i = 0; i < menu->size_y; i++) {
		moveCursor(menu->ref_x + 2, menu->ref_y + 1 + i);
		wprintf(L"%ls %ls", ((struct field_t*)menu->items)[i].field_name, ((struct field_t*)menu->items)[i].field_value);
	}
	return 0;
//	moveCursor(menu->ref_x + 2 + wcslen(menu->items[item_index]->pretty_name), menu->ref_y + 1 + item_index);
//	wprintf(data);
	return 0;
}

int clear_style(struct menu_t *menu) {
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);
	wprintf(L"\033[0m%*s", menu->size_x + 1, "");
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);
	wprintf(L"\033[0m %ls", ((struct menu_t**)menu->items)[menu->cur_y - 1]->pretty_name);

	return 0;
}

int set_style(struct menu_t *menu) {
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);
	wprintf(L"\033[47m%*s", menu->size_x + 1, "");
	moveCursor(menu->ref_x + 1, menu->ref_y + menu->cur_y);
	wprintf(L"\033[30m %ls", ((struct menu_t**)menu->items)[menu->cur_y - 1]->pretty_name);

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
	size_t max = 0;
	size_t len = 0;

	for (int i = 0; i < menu->size_y; i++) {
		len = wcslen(((struct menu_t**)menu->items)[i]->pretty_name);
		if (len > max) {
			max = len;
		}
	}

	return (int)max;
}
/*
wchar_t *category_menu_items[] = {
	L"Torrent Info",
	L"Tracker Info",
	L"Meta Data",
	L"Files"
};
*/
struct menu_t *category_menu_items[] = {
	&torrent_info_menu,
	&tracker_info_menu
//	meta_data_menu,
//	files_menu
};


struct menu_t category_menu = {
	.pretty_name = L"N/A",
	.items = (void *)category_menu_items,
	.type = MENU,
	.ref_x = 1,
	.ref_y = 1,
	.item_index = 0,
	.size_y = 2,
	.prev_menu = NULL,
	.cur_x = 0,
	.cur_y = 0
};
