#ifndef H_MENU_TEMPLATES
#define H_MENU_TEMPLATES

#include "tui.h"

extern struct menu_t category_menu;
extern struct menu_t *category_menu_items[];

extern struct menu_t torrent_info_menu;
extern struct field_t torrent_info_menu_items_template[];
extern struct field_t **torrent_info_menu_items;

extern struct menu_t tracker_info_menu;
extern struct field_t **tracker_info_menu_items;

extern struct menu_t meta_info_menu;
extern struct field_t meta_info_menu_items_template[];
extern struct field_t **meta_info_menu_items;

extern struct menu_t files_menu;
extern struct field_t **files_menu_items;

#endif
