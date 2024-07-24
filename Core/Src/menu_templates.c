#include <wchar.h>

#include "../Inc/menu.h"
#include "../Inc/menu_templates.h"

struct menu_t *category_menu_items[] = {
	&torrent_info_menu,
	&tracker_info_menu,
	&meta_data_menu
//	files_menu
};

struct menu_t category_menu = {
	.pretty_name 	= NULL,
	
	.items 			= (void *)category_menu_items,
	.type 			= MENU,
	.prev_menu 		= NULL,

	.ref_x 			= 1,
	.ref_y 			= 1,
	
	.size_x			= 1,
	.size_y 		= 3,
	
	.cur_x 			= 1,
	.cur_y 			= 1
};


struct field_t **torrent_info_menu_items;
struct menu_t torrent_info_menu = {
	.pretty_name 	= L"Torrent Info",
	
	.items 			= NULL,
	.type 			= FIELD,
	.prev_menu 		= &category_menu,
	
	.ref_x 			= 1,
	.ref_y 			= 1,
	
	.size_x			= 1,
	.size_y 		= 2,
	
	.cur_x 			= 1,
	.cur_y 			= 1
};
struct field_t torrent_info_menu_items_template[] = {
	{L"Filename :", NULL},
	{L"Info Hash:", NULL}
};

struct field_t **tracker_info_menu_items;
struct menu_t tracker_info_menu = {
	.pretty_name 	= L"Tracker Info",

	.items 			= NULL,
	.type 			= FIELD,
	.prev_menu 		= &category_menu,
	
	.ref_x 			= 1,
	.ref_y 			= 1,
	
	.size_x			= 1,
	.size_y 		= 0,
	
	.cur_x 			= 1,
	.cur_y 			= 1
};


struct field_t **meta_data_menu_items;
struct menu_t meta_data_menu = {
	.pretty_name	= L"Meta Data",

	.items			= NULL,
	.type			= FIELD,
	.prev_menu		= &category_menu,
	
	.ref_x			= 1,
	.ref_y			= 1,
	
	.size_x			= 1,
	.size_y			= 0,

	.cur_x			= 1,
	.cur_y			= 1
};
struct field_t meta_data_menu_items_template[] = {
	{L"Directory   :", NULL},
	{L"Created On  :", NULL},
	{L"Created By  :", NULL},
	{L"Comment     :", NULL},
	{L"Piece Length:", NULL},
	{L"Private     :", NULL}
};
