#include "tui.h"

#ifndef H_MENU
#define H_MENU

extern wchar_t *category_menu_items[];
extern int category_menu_size;
extern struct menu_t category_menu;

extern wchar_t *torrent_info_menu_items[];
extern int torrent_info_menu_size;
extern struct menu_t torrent_info_menu;

extern wchar_t *tracker_info_menu_items[];
extern int tracker_info_menu_size;
extern struct menu_t tracker_info_menu;

extern wchar_t *meta_data_menu_items[];
extern int meta_data_menu_size;
extern struct menu_t meta_data_menu;

extern wchar_t *files_menu_items[];
extern int files_menu_size;
extern struct menu_t files_menu;

int max_size(struct menu_t*);

int resize_menu(struct menu_t*);

int draw_box(int, int, int, int);
int draw_menu(struct menu_t*);
int draw_data(struct menu_t*, wchar_t*, int);

int clear_style(struct menu_t*);
int set_style(struct menu_t*);

#endif
