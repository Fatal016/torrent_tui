#include <wchar.h>

#include "../Inc/menu.h"

#define category_max_size max_size(category_menu_items, category_menu_size)

const wchar_t *category_menu_items[] = {
	L"Torrent Info",
	L"Tracker Info",
	L"Meta Data",
	L"Files"
};

struct menu_t category_menu = {
	.items = category_menu_items,
	.ref_x = 1,
	.ref_y = 1,
	.item_index = 0,
	.size_x = 13,
	.size_y = 4,
	.prev_menu = NULL,
	.cur_x = 0,
	.cur_y = 0
};


const wchar_t *torrent_info_menu_items[] = {
	L"Filename",
	L"Info Hash"
};

struct menu_t torrent_info_menu = {
	.items = torrent_info_menu_items,
	.ref_x = 1,
	.ref_y = 1,
	.item_index = 0,
	.size_x = 10,
	.size_y = 2,
	.prev_menu = &category_menu,
	.cur_x = 0,
	.cur_y = 0
};

int max_size(const wchar_t** items, const int len)
{
/*
    int max = 0;

    for (int i = 0; i < len; i++) {
        if (strlen(items[i]) > max) {
            max = strlen(items[i]);
        }
    }

    return max;
*/
    return 13;
}
