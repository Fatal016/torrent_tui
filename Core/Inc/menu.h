#include "tui.h"

#ifndef H_MENU
#define H_MENU

extern const wchar_t *category_menu_items[];
extern const int category_menu_size;
extern struct menu_t category_menu;

extern const wchar_t *torrent_info_menu_items[];
extern const int torrent_info_menu_size;
extern struct menu_t torrent_info_menu;

extern const wchar_t *tracker_info_menu_items[];
extern const int tracker_info_menu_size;
extern struct menu_t tracker_info_menu;

extern const wchar_t *meta_data_menu_items[];
extern const int meta_data_menu_size;
extern struct menu_t meta_data_menu;

extern const wchar_t *files_menu_items[];
extern const int files_menu_size;
extern struct menu_t files_menu;

int max_size(const wchar_t**, const int);

#endif
