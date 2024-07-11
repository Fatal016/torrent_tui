#include "tui.h"

#ifndef H_MENU_TEMPLATES
#define H_MENU_TEMPLATES

extern struct menu_t category_menu;
extern struct menu_t *category_menu_items[];
extern int category_menu_size;

extern struct menu_t torrent_info_menu;
extern struct field_t torrent_info_menu_items[];
extern int torrent_info_menu_size;

extern wchar_t *tracker_info_menu_items[];
extern int tracker_info_menu_size;
extern struct menu_t tracker_info_menu;

extern wchar_t *meta_data_menu_items[];
extern int meta_data_menu_size;
extern struct menu_t meta_data_menu;

extern wchar_t *files_menu_items[];
extern int files_menu_size;
extern struct menu_t files_menu;

#endif
