#include <wchar.h>

#include "../Inc/menu.h"
#include "../Inc/menu_templates.h"

struct menu_t *category_menu_items[] = {
	&torrent_info_menu,
	&tracker_info_menu
//	meta_data_menu,
//	files_menu
};

struct menu_t category_menu = {
	.pretty_name 	= L"N/A",
	.items 			= (void *)category_menu_items,
	.type 			= MENU,
	.ref_x 			= 1,
	.ref_y 			= 1,
	.item_index 	= 0,
	.size_y 		= 2,
	.prev_menu 		= NULL,
	.cur_x 			= 0,
	.cur_y 			= 0
};



struct field_t torrent_info_menu_items_template[] = {
	{L"Filename :", NULL},
	{L"Info Hash:", NULL}
};


struct field_t **torrent_info_menu_items;



struct menu_t torrent_info_menu = {
	.pretty_name 	= L"Torrent Info",
	.items 			= NULL,
	.type 			= FIELD,
	.nature			= DYNAMIC,
	.ref_x 			= 1,
	.ref_y 			= 1,
	.item_index 	= 0,
	.size_y 		= 2,
	.prev_menu 		= &category_menu,
	.cur_x 			= 1,
	.cur_y 			= 1
};

struct field_t **tracker_info_menu_items;

struct menu_t tracker_info_menu = {
	.pretty_name 	= L"Tracker Info",
	.items 			= NULL,
	.type 			= FIELD,
	.nature			= DYNAMIC,
	.ref_x 			= 1,
	.ref_y 			= 1,
	.item_index 	= 0,
	.size_y 		= 0,
	.prev_menu 		= &category_menu,
	.cur_x 			= 1,
	.cur_y 			= 1
};
